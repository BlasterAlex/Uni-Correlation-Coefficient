#include <QEventLoop>
#include <QFile>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>
#include <QWidget>

#include "WebLoader.hpp"

WebLoader::WebLoader(QWidget *parent) : QDialog(parent) {

  // Рейтинги
  webView = new QWebEngineView(this);
  connect(webView, SIGNAL(loadFinished(bool)), SLOT(readPage(bool)));

  QWebChannel *channel = new QWebChannel(this);
  channel->registerObject("api", this);
  webView->page()->setWebChannel(channel);

  webView->load(
      QUrl("https://www.timeshighereducation.com/world-university-rankings/2019/subject-ranking/engineering-and-IT#!/page/0/length/-1/sort_by/"));
  webView->hide();

  // Page
  view = new QWebEngineView(this);
  view->load(QUrl("qrc:/html/html/page.html"));

  QVBoxLayout *viewLayout = new QVBoxLayout;
  viewLayout->addWidget(view);

  setLayout(viewLayout);
}

void WebLoader::readPage(bool) {
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

  // Вызов функции из js
  webView->page()->runJavaScript("getTable('.table tbody tr, .rank, .name .ranking-institution-title')");
}

void WebLoader::endOfTable() {
  foreach (QList<QVariant> row, someTable)
    qDebug() << row[0].toString() << ": " << row[1].toString();

  view->page()->runJavaScript("addStep();");
}