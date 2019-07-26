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

#include <QCollator>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSpacerItem>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>
#include <QWidget>
#include <algorithm>

#include "../../Table/Table.hpp"
#include "../../blocks/HoverButton/HoverButton.hpp"
#include "../../settings/settings.hpp"
#include "WebLoader.hpp"

WebLoader::WebLoader(QWidget *parent) : QDialog(parent) {

  // Заголовок
  setWindowTitle("Загрузка файлов");
  setMinimumWidth(300);

  // Основной слой
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(8, 8, 8, 10);
  mainLayout->setSpacing(8);

  // Полоска загрузки
  QGroupBox *progress = new QGroupBox(this);
  QVBoxLayout *vbox = new QVBoxLayout(progress);

  progress_bar = new QProgressBar(progress);
  progress_bar->setMinimum(1);
  progress_bar->setMaximum(int());
  vbox->addWidget(progress_bar);

  mainLayout->addWidget(progress);

  // Текстовые поля
  QHBoxLayout *textLayout = new QHBoxLayout;
  textLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  progress_indicator = new QLabel(progress);
  progress_indicator->setText("|");
  progress_indicator->setFixedWidth(12);
  textLayout->addWidget(progress_indicator);

  progress_label = new QLabel(progress);
  progress_label->setText("Загрузка...");
  progress_label->setStyleSheet("margin-top: 1px;");
  textLayout->addWidget(progress_label);

  mainLayout->addLayout(textLayout);

  // Кнопка выхода
  exitButton = new HoverButton("Выйти", this);
  exitButton->setStyleSheet("width: 120px; height: 15px;");
  exitButton->setFocusPolicy(Qt::NoFocus);
  mainLayout->addWidget(exitButton, 0, Qt::AlignTop | Qt::AlignHCenter);
  exitButton->hide();
  connect(exitButton, &HoverButton::clicked, this, &WebLoader::leave);

  setLayout(mainLayout);
}

void WebLoader::filesUpload() { // загрузка всех файлов

  // Отключение ненавязчивых вопросов
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Замена файлов", "Можно перезаписывать существующие файлы?", QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes)
    ask = false;
  else
    ask = true;

  // Чтение заголовков ресурсов
  QVariant resHeaders = getWebRes("info/resources");
  if (strcmp(resHeaders.typeName(), "QString") == 0)
    resources.push_back(resHeaders.toString());
  else if (strcmp(resHeaders.typeName(), "QStringList") == 0) {
    // Массив заголовков
    resources = resHeaders.toStringList().toVector();

    // Сортировка по имени и номерам
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(resources.begin(), resources.end(), [&collator](const QString &a, const QString &b) { return collator.compare(a, b) < 0; });
  }

  // Подсчет количества шагов
  progress_bar->setMaximum(resources.size() * 4); // количество таблиц * 4

  // Запуск таймера
  timer = new QTimer(this);
  timer->setInterval(300);
  connect(timer, &QTimer::timeout, this, &WebLoader::updateIndicator);
  timer->start();

  // Отслеживание события завершения загрузки таблицы
  connect(this, &WebLoader::tableDone, this, &WebLoader::goToNextTable);

  // Запуск процесса
  fileUpload();
  return;
}

// Переход к следующей таблице
void WebLoader::goToNextTable() {
  resources.remove(0);

  if (resources.empty()) { // это была последняя таблица
    qDebug() << "Загрузка завершена";
    setLabelText("Загрузка завершена");
    done = true;
    if (webV)
      delete webView;
    timer->stop();
    exitButton->show();
  } else
    fileUpload();
}

// Обновление индикатора
void WebLoader::updateIndicator() {
  static int count = 0;
  QVector<QString> indicators = {"/", "−", "\\", "|"};
  if (count == 4)
    count = 0;
  progress_indicator->setText(indicators[count++]);
}

// Разрешение на закрытие окна
bool WebLoader::completenessCheck() {
  if (done)
    return true;
  else {
    QMessageBox::critical(this, "Загрузка не завершена", "Процесс загрузки еще не завершился, пожалуйста подождите");
    return false;
  }
}

// Событие закрытия окна
void WebLoader::reject() {
  if (completenessCheck()) {
    emit shutdown();
    QDialog::reject();
  }
}

// Закрытие окна по нажатию
void WebLoader::leave() { reject(); }