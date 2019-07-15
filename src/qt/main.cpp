#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QWidget>

#include "FileUpload/FileUpload.hpp"
#include "settings/settings.hpp"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(getSetting("app/ico").toString()));

  FileUpload *w = new FileUpload;
  w->setAttribute(Qt::WA_DeleteOnClose);
  w->show();

  return app.exec(); // запускаем цикл обработки сообщений
}