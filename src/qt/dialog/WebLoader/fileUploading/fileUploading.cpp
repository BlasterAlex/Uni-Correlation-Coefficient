/***
 * Copyright 2019 Alexander Pishchulev (https://github.com/BlasterAlex)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>
#include <QWebChannel>
#include <QWebEngineView>

#include "../../../Table/Table.hpp"
#include "../../../settings/settings.hpp"
#include "../WebLoader.hpp"

void WebLoader::fileUpload() { // загрузка одного файла
  QString url = getWebRes(resources[0] + "/href").toString();
  setLabelText("Проверка url для " + resources[0]);

  if (!urlExists(url)) { // проверка доступности url
    setLabelText("Url для " + resources[0] + " не доступен!");

    if (resources[0] != "ARWU1") // если это вся таблица
      progress_bar->setValue(progress_bar->value() + 4);
    else
      progress_bar->setValue(progress_bar->value() + 8);

    emit tableDone();
  } else {
    setLabelText("Url для " + resources[0] + " доступен");
    progress_bar->setValue(progress_bar->value() + 1);

    setLabelText("Чтение таблицы " + resources[0]);
    readPage();
  }
}

bool WebLoader::urlExists(QString url_string) { // проверка url на доступность
  QUrl url(url_string);
  QTcpSocket socket;
  socket.connectToHost(url.host(), 80);
  if (socket.waitForConnected()) {
    socket.write("HEAD " + url.path().toUtf8() +
                 " HTTP/1.1\r\n"
                 "Host: " +
                 url.host().toUtf8() + "\r\n\r\n");
    if (socket.waitForReadyRead()) {
      QByteArray bytes = socket.readAll();
      if (bytes.contains("200 OK") || bytes.contains("302 Found") || bytes.contains("301 Moved")) {
        return true;
      }
    }
  }
  return false;
}

void WebLoader::readPage() { // чтение очередной страницы

  QString url = getWebRes(resources[0] + "/href").toString();

  // Очистка хранимых данных
  if (resources[0] != "ARWU2")
    someTable.clear();

  if (webV)
    delete webView;
  else
    webV = true;

  // Создание отображения страницы
  webView = new QWebEngineView(this);
  connect(webView, SIGNAL(loadFinished(bool)), SLOT(loadFinished(bool)));

  // Настройка канала для коммуникации с js файлом
  QWebChannel *channel = new QWebChannel(this);
  channel->registerObject("api", this);
  webView->page()->setWebChannel(channel);

  // Загрузка страницы
  webView->load(QUrl(url));
  webView->hide();
}

void WebLoader::loadFinished(bool) { // запуск скриптов после загрузки страницы

  // Загрузка webchannel
  QFile fileChannel;
  fileChannel.setFileName("resources/js/qwebchannel.js");
  fileChannel.open(QIODevice::ReadOnly);
  QString webchannel = fileChannel.readAll();
  fileChannel.close();
  webView->page()->runJavaScript(webchannel);

  // Загрузка Jquery
  QFile fileJquery;
  fileJquery.setFileName("resources/js/jquery-3.4.1.min.js");
  fileJquery.open(QIODevice::ReadOnly);
  QString jquery = fileJquery.readAll();
  fileJquery.close();
  webView->page()->runJavaScript(jquery);

  // Загрузка js
  QFile fileJS;
  fileJS.setFileName("resources/js/raitingPage.js");
  fileJS.open(QIODevice::ReadOnly);
  QString js = fileJS.readAll();
  fileJS.close();
  webView->page()->runJavaScript(js);

  if (resources[0] != "ARWU2") // если это вся таблица
    // Вызов функции из js
    webView->page()->runJavaScript("getTable('" + getWebRes(resources[0] + "/headline").toString() + ", " +
                                   getWebRes(resources[0] + "/rank").toString() + ", " + getWebRes(resources[0] + "/name").toString() + "')");
  else // если это вторая часть таблицы
    // Вызов функции из js
    webView->page()->runJavaScript("getSecondPart('" + getWebRes(resources[0] + "/headline").toString() + ", " +
                                   getWebRes(resources[0] + "/rank").toString() + ", " + getWebRes(resources[0] + "/name").toString() + "')");
}

void WebLoader::setLabelText(QString msg) { // вывод сообщения
  progress_label->setText(msg);
}

void WebLoader::addTableRow(QList<QVariant> arg) { // получение строки таблицы
  someTable.append(arg);
}

void WebLoader::endOfTable() { // чтение таблицы завершено

  setLabelText("Таблица " + resources[0] + " получена");
  progress_bar->setValue(progress_bar->value() + 1);

  if (someTable.size() == 0 || someTable.size() == 1) {
    timer->stop();
    QMessageBox::critical(this, "Ошибка загрузки", "Загруженная таблица " + resources[0] + " пустая. Пропускается");

    setLabelText("Переход к следующему файлу");
    progress_bar->setValue(progress_bar->value() + 2);

    timer->start();
    emit tableDone();
  } else {
    if (resources[0] != "ARWU1") { // если это вся таблица
      // Запись полученной таблицы в файл
      QString filename;
      if (resources[0] != "ARWU2")
        filename = resources[0] + ".csv";
      else
        filename = resources[0].left(resources[0].length() - 1) + ".csv";

      Table table(someTable, filename);
      progress_bar->setValue(progress_bar->value() + 1);

      QString target = getSetting("uploads/dir").toString() + QDir::separator();
      if (QFile::exists(target)) {

        if (QFile::exists(target + filename)) { // такой файл уже есть
          if (ask) {
            timer->stop();
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, "Файл существует", "Файл '" + filename + "' уже существует!\nМожно переписать этот файл?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
              emit fileDelRequest(filename); // удаление файла
              timer->start();
            } else {
              setLabelText("Переход к следующему файлу");
              progress_bar->setValue(progress_bar->value() + 1);

              timer->start();
              emit tableDone();
              return;
            }
          } else
            emit fileDelRequest(filename); // удаление файла
        }
        progress_bar->setValue(progress_bar->value() + 1);
        if (table.createNewFile()) {
          setLabelText("Файл " + filename + " успешно сохранен");
          emit fileAddRequest(filename);
        } else {
          setLabelText("Ошибка загрузки файла " + filename);
        }

        emit tableDone();
      }
    } else { // таблица разбита на две части
      progress_bar->setValue(progress_bar->value() + 1);
      resources[0] = "ARWU2"; // имя второй таблицы
      progress_bar->setValue(progress_bar->value() + 1);
      fileUpload();
    }
  }
}