#ifndef UPLOAD_H
#define UPLOAD_H

#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QStringList>
#include <QVector>
#include <QWidget>

#include "../blocks/FileBlock/FileBlock.hpp"
#include "../blocks/UploadForm/UploadForm.hpp"
#include "../dialog/Coefficients/Coefficients.hpp"
#include "../dialog/WebLoader/WebLoader.hpp"

class FileUpload : public QWidget {
  Q_OBJECT

public:
  explicit FileUpload(QWidget *parent = 0);
  virtual ~FileUpload() {
    for (auto fileBlock : fileList)
      delete fileBlock;
    fileList.clear();
  }

protected:
  void addFile(QString); // добавление нового файла

private slots:
  void formSubmited(); // загрузка файлов с формы
  void removeFile();   // удаление файла из списка
  void submit();       // отправка данных
  void noDialog();     // обновить состояние диалогового окна
  void callPage();

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

  WebLoader *page;

  void createDragAndDrop();      // создание поля загрузки файлов
  void createFileList();         // вывод списка загруженных файлов
  bool checkExtensions();        // проверка файлов на тип
  int searchFileByName(QString); // поиск файла по имени
  void removeFile(QString);      // удаление файла по имени

  QVector<Table> getParsedTables(QVector<QString>); // чтение таблиц из файлов
};

#endif // UPLOAD_H
