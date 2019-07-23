#ifndef WEBLOADER_H
#define WEBLOADER_H

#include <QCloseEvent>
#include <QDialog>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QProgressBar>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QWebEngineView>
#include <QWidget>

#include "../../blocks/HoverButton/HoverButton.hpp"

class WebLoader : public QDialog {
  Q_OBJECT

public:
  explicit WebLoader(QWidget *parent = 0);
  virtual ~WebLoader() {}

protected:
  void reject(); // закрытие окна

private:
  // Полоска загрузки
  QLabel *progress_label;
  QProgressBar *progress_bar;

  // Вращающийся индикатор загрузки
  QTimer *timer;
  QLabel *progress_indicator;

  // Страница ресурса
  bool webV = false;
  QWebEngineView *webView;

  // Названия ресурсов для загрузки
  QVector<QString> resources = {"ARWU1", "CWUR", "RUR", "THE"};

  // Хранимая таблица рейтингов
  QList<QList<QVariant>> someTable;

  // Спрашивать при замене файлов
  bool ask;

  // Состояние процесса загрузки
  bool done = false;

  // Кнопка завершения
  HoverButton *exitButton;

  void filesUpload();       // загрузка всех файлов
  void fileUpload();        // загрузка одного файла
  bool urlExists(QString);  // проверка url на доступность
  void readPage();          // чтение очередной страницы
  bool completenessCheck(); // разрешение на закрытие окна

private slots:
  void loadFinished(bool); // загрузка страницы завершена
  void goToNextTable();    // переход к следующей таблице
  void updateIndicator();  // обновление индикатора
  void leave();            // закрытие окна по нажатию

public slots:
  void addTableRow(QList<QVariant>); // получение строки таблицы
  void endOfTable();                 // чтение таблицы завершено
  void setLabelText(QString);        // вывод сообщения

signals:
  void tableDone();             // окончание загрузки таблицы
  void fileAddRequest(QString); // добавление файла в главную форму
  void fileDelRequest(QString); // удаление файла из главной формы
  void shutdown();              // событие закрытия окна

  friend class FileUpload;
};

#endif // WEBLOADER_H