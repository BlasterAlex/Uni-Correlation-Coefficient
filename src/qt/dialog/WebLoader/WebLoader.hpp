#ifndef WEBLOADER_H
#define WEBLOADER_H

#include <QDialog>
#include <QList>
#include <QMainWindow>
#include <QString>
#include <QWebEngineView>
#include <QWidget>

class WebLoader : public QDialog {
  Q_OBJECT

  QList<QList<QVariant>> someTable;

public:
  explicit WebLoader(QWidget *parent = 0);
  virtual ~WebLoader() {}

private slots:
  void readPage(bool);

public slots:
  void message(QVariant text) { qDebug() << text.toString(); }
  void endOfTable();
  void addTableRow(QList<QVariant> arg) { someTable.append(arg); }

private:
  QWebEngineView *view;
  QWebEngineView *webView;

signals:
  void sendMessage(const QString &text);
};

#endif // WEBLOADER_H