#ifndef UPLOAD_H
#define UPLOAD_H

#include <QGroupBox>
#include <QLabel>
#include <QStringList>
#include <QVector>
#include <QWidget>

#include "../../Coefficients/Coefficients.hpp"
#include "../../blocks/FileBlock/FileBlock.hpp"
#include "../../blocks/UploadForm/UploadForm.hpp"

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

  void createDragAndDrop();      // создание поля загрузки файлов
  void createFileList();         // вывод списка загруженных файлов
  bool checkExtensions();        // проверка файлов на тип
  int searchFileByName(QString); // поиск файла по имени
  void removeFile(QString);      // удаление файла по имени
};

#endif // UPLOAD_H
