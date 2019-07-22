#ifndef UPLOAD_H
#define UPLOAD_H

#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QMainWindow>
#include <QStringList>
#include <QVector>
#include <QWidget>

#include "../blocks/FileBlock/FileBlock.hpp"
#include "../blocks/UploadForm/UploadForm.hpp"
#include "../dialog/Coefficients/Coefficients.hpp"
#include "../dialog/WebLoader/WebLoader.hpp"

class FileUpload : public QMainWindow {
  Q_OBJECT

public:
  explicit FileUpload(QWidget *parent = 0);
  virtual ~FileUpload() {
    for (auto fileBlock : fileList)
      delete fileBlock;
    fileList.clear();
  }

protected:
  void closeEvent(QCloseEvent *); // закрытие окна

private slots:
  void formSubmited();      // загрузка файлов с формы
  void addFile(QString);    // добавление нового файла
  void removeFile();        // удаление файла из списка
  void removeFile(QString); // удаление файла по имени
  void submit();            // отправка данных
  void callPage();          // вызов окна загрузки файлов
  void noDialog();          // обновить состояние диалогового окна
  void noWebDialog();       // обновить состояние диалогового окна загрузки

private:
  // Поле для загрузки файлов
  UploadForm *dragAndDrop;

  // Список загруженных файлов
  QGroupBox *fileListBlock;

  // Массив файлов списка
  QVector<FileBlock *> fileList;

  // Окно результатов вычислений
  Coefficients *coeff;
  bool dialog = false;

  // Скачивание новых таблиц
  WebLoader *page;
  bool web = false;

  void createDragAndDrop();      // создание поля загрузки файлов
  void createFileList();         // вывод списка загруженных файлов
  bool checkExtensions();        // проверка файлов на тип
  int searchFileByName(QString); // поиск файла по имени

  QVector<Table> getParsedTables(QVector<QString>); // чтение таблиц из файлов
};

#endif // UPLOAD_H
