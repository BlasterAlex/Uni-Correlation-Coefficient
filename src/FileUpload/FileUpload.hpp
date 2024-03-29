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

#ifndef UPLOAD_H
#define UPLOAD_H

#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QMainWindow>
#include <QStringList>
#include <QToolButton>
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
  void resizeEvent(QResizeEvent *); // изменение размера окна
  void closeEvent(QCloseEvent *);   // закрытие окна

private slots:
  void formSubmited();      // загрузка файлов с формы
  void refreshList();       // обновление списка файлов
  void addFile(QString);    // добавление нового файла
  void removeFile();        // удаление файла из списка
  void removeFile(QString); // удаление файла по имени
  void submit();            // отправка данных
  void callWeb();           // вызов окна загрузки файлов
  void noDialog();          // обновить состояние диалогового окна
  void noWebDialog();       // обновить состояние диалогового окна загрузки

private:
  // Меню
  QGroupBox *menu;
  QToolButton *dwnMenu;

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

  void createMenu();             // создание меню
  void createDragAndDrop();      // создание поля загрузки файлов
  void createFileList();         // вывод списка загруженных файлов
  bool checkExtensions();        // проверка файлов на тип
  int searchFileByName(QString); // поиск файла по имени

  QVector<Table> getParsedTables(QVector<QString>); // чтение таблиц из файлов
};

#endif // UPLOAD_H
