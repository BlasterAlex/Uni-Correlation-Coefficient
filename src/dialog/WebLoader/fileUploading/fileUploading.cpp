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
#include <QVector>
#include <QWebChannel>
#include <QWebEngineView>
#include <algorithm>

#include "../../../Table/Table.hpp"
#include "../../../settings/settings.hpp"
#include "../WebLoader.hpp"

bool isFullTable(QString name) { // проверка на тип таблицы
  if (nameParsing(name).number == -1)
    return true;
  else
    return false;
}

bool isFirstPart(QString name) { // проверка на первую часть таблицы
  if (isFullTable(name) || nameParsing(name).number == 1)
    return true;
  else
    return false;
}

bool WebLoader::isLastPart(QString name) { // проверка на последнюю часть таблицы

  // Получение типа таблицы
  if (isFullTable(name))
    return true;

  // Разбор названия таблицы
  WebRes res = nameParsing(name);

  // Поиск следующей части таблицы
  res.number++;
  QString fullName = res.getFullName();

  auto found = std::find_if(resources.begin(), resources.end(), [&np = fullName](const QString &resource) { return resource == np; });

  if (found != resources.end()) // это не последняя часть таблицы
    return false;

  return true; // это последняя часть таблицы
}

void WebLoader::fileUpload() { // загрузка одного файла

  QString url = getWebRes(resources[0] + "/href").toString();
  setLabelText("Проверка url для " + resources[0]);

  if (!urlExists(url)) { // проверка доступности url

    // Url не доступен
    setLabelText("Url для " + resources[0] + " не доступен!");

    // Шага на одну таблицу
    int stepsToTheNextTable = 4;

    // Определение количества таблиц до следующей
    while (isLastPart(resources[0]) != true) {
      resources.remove(0);
      stepsToTheNextTable += 4;
    }

    // Установка нового значения лодера
    progress_bar->setValue(progress_bar->value() + stepsToTheNextTable);

    emit tableDone();
  } else {

    // Url доступен
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

  // Очистка хранимых данных, если это целая таблица или первая часть таблицы
  if (isFirstPart(resources[0]))
    someTable.clear();

  // Удаление веб-страницы, если она открыта
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

  if (isFirstPart(resources[0])) // если это вся таблица или первая часть
    // Вызов функции из js
    webView->page()->runJavaScript("getTable('" + getWebRes(resources[0] + "/headline").toString() + ", " +
                                   getWebRes(resources[0] + "/rank").toString() + ", " + getWebRes(resources[0] + "/name").toString() + "')");
  else // если это другая часть таблицы
    // Вызов функции из js
    webView->page()->runJavaScript("getNextPart('" + getWebRes(resources[0] + "/headline").toString() + ", " +
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

  if (someTable.size() == 0 || someTable.size() == 1) { // пришла пустая таблица рейтингов
    timer->stop();
    QMessageBox::critical(this, "Ошибка загрузки", "Загруженная таблица " + resources[0] + " пустая. Пропускается");

    setLabelText("Переход к следующему файлу");
    progress_bar->setValue(progress_bar->value() + 2);

    timer->start();
    emit tableDone();
  } else {
    if (isLastPart(resources[0])) { // если это вся таблица

      // Запись полученной таблицы в файл
      QString filename = nameParsing(resources[0]).name + ".csv";
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
    } else { // таблица разбита на части
      setLabelText("Переход к следующей части таблицы");
      progress_bar->setValue(progress_bar->value() + 2);
      emit tableDone();
    }
  }
}