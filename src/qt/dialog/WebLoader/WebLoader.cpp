#include <QCloseEvent>
#include <QEventLoop>
#include <QFile>
#include <QGroupBox>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>
#include <QThread>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>
#include <QWidget>

#include "../../settings/settings.hpp"
#include "WebLoader.hpp"

WebLoader::WebLoader(QWidget *parent) : QDialog(parent) {

  // Заголовок
  setWindowTitle("Загрузка файлов");

  // Основной слой
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(8, 8, 8, 8);
  mainLayout->setSpacing(0);

  // Полоска загрузки
  QGroupBox *progress = new QGroupBox(this);
  QVBoxLayout *vbox = new QVBoxLayout(progress);

  progress_bar = new QProgressBar(progress);
  progress_bar->setMinimum(1);
  progress_bar->setMaximum(getWebRes("info/quantity").toInt() * 4); // количество таблиц * 4
  vbox->addWidget(progress_bar);

  progress_label = new QLabel(progress);
  progress_label->setText("Загрузка...");
  vbox->addWidget(progress_label);
  mainLayout->addWidget(progress);

  setLayout(mainLayout);
}

void WebLoader::filesUpload() { // загрузка всех файлов

  // Отключение ненавязчивых вопросов
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Вопрос", "Можно перезаписывать существующие файлы?", QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes)
    ask = false;
  else
    ask = true;

  // Отслеживание события завершения загрузки таблицы
  connect(this, &WebLoader::tableDone, this, &WebLoader::goToNextTable);

  // Запуск процесса
  fileUpload();
  return;
}

// Переход к следующей таблице
void WebLoader::goToNextTable() {
  resources.remove(0);

  if (resources.empty()) {
    qDebug() << "Done";
    setLabelText("Загрузка завершена");

    done = true;
    if (webV)
      delete webView;
  } else
    fileUpload();
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
