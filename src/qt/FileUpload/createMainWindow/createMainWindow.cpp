#include <QDir>
#include <QFile>
#include <QFormLayout>
#include <QMessageBox>
#include <QString>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "../../blocks/FileBlock/FileBlock.hpp"
#include "../../blocks/HoverButton/HoverButton.hpp"
#include "../../blocks/UploadForm/UploadForm.hpp"
#include "../../settings/settings.hpp"
#include "../FileUpload.hpp"

#define sendBlock qobject_cast<FileBlock *>
#define sendAction qobject_cast<QToolButton *>

// Создание меню
void FileUpload::createMenu() {
  menu = new QGroupBox(this);
  menu->setStyleSheet("QGroupBox {"
                      "  border: none;"
                      "  border-bottom: 2px solid rgba(178, 178, 178, 178);"
                      "  margin: 0 0 10px 0;"
                      "  text-align: center;"
                      "  min-width: 700px;"
                      "}");

  QVBoxLayout *vbox = new QVBoxLayout(menu);
  vbox->setMargin(0);
  vbox->setContentsMargins(0, 0, 0, 0);
  vbox->setSpacing(0);

  QToolBar *toolBar = new QToolBar(menu);

  // Кнопки действий
  dwnMenu = new QToolButton(toolBar);
  dwnMenu->setText("&Скачать");
  dwnMenu->setStyleSheet("color: black");
  dwnMenu->setFocusPolicy(Qt::NoFocus);
  dwnMenu->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));
  toolBar->addWidget(dwnMenu);
  connect(dwnMenu, &QToolButton::clicked, this, &FileUpload::callWeb);

  QToolButton *refMenu = new QToolButton(toolBar);
  refMenu->setText("&Обновить");
  refMenu->setStyleSheet("color: black");
  refMenu->setFocusPolicy(Qt::NoFocus);
  refMenu->setShortcut(QKeySequence(Qt::ALT + Qt::Key_J));
  toolBar->addWidget(refMenu);
  connect(refMenu, &QToolButton::clicked, this, &FileUpload::refreshList);

  toolBar->setLayoutDirection(Qt::LeftToRight);
  vbox->addWidget(toolBar);
}

// Создание поля загрузки файлов
void FileUpload::createDragAndDrop() {
  dragAndDrop = new UploadForm(this);
  connect(dragAndDrop, SIGNAL(submit()), this, SLOT(formSubmited()));
}

// Вывод списка загруженных файлов
void FileUpload::createFileList() {
  fileListBlock = new QGroupBox(tr("Загруженные файлы"), this);
  fileListBlock->setMinimumWidth(430);
  fileListBlock->setObjectName("fileListBlock");
  fileListBlock->setStyleSheet("#fileListBlock { margin: 20px 15px 0 15px; padding: 10px 0; }"
                               ""
                               "#fileListBlock::title { padding: -10px 15px 15px 10px }");

  QString dirName = getSetting("uploads/dir").toString();
  QDir dir(dirName);
  if (!dir.exists()) {
    QDir().mkdir(dirName);
    qDebug() << "Создана папка " + dirName;
  }

  QFormLayout *vbox = new QFormLayout(fileListBlock);
  vbox->setContentsMargins(5, 5, 10, 5);

  foreach (QString file, dir.entryList(QDir::Files)) {
    QFileInfo fileInfo(file);
    QString filename = fileInfo.fileName();

    FileBlock *block = new FileBlock(filename, this);

    connect(block, SIGNAL(remove()), this, SLOT(removeFile()));
    fileList.push_back(block);
    vbox->addWidget(block);
  }

  if (!fileList.size())
    fileListBlock->hide(); // если нет файлов
  else {
    checkExtensions(); // проверка, если есть файлы
    fileListBlock->setFixedHeight(fileList.size() * 63 + 30);
  }
}

// Добавление нового файла
void FileUpload::addFile(QString file) {
  QFileInfo fileInfo(file);
  QString filename = fileInfo.fileName();
  FileBlock *block = new FileBlock(filename, this);

  connect(block, SIGNAL(remove()), this, SLOT(removeFile()));
  fileList.push_back(block);
  fileListBlock->layout()->addWidget(block);

  if (fileList.size()) {
    fileListBlock->show();
    fileListBlock->setFixedHeight(fileList.size() * 63 + 30);
  }
}

// Поиск файла по имени
int FileUpload::searchFileByName(QString keyName) {
  int i = 0;
  foreach (FileBlock *fileBlock, fileList) {
    if (fileBlock->text.compare(keyName) == 0)
      return i;
    ++i;
  }
  return -1;
}

// Удаление файла из списка
void FileUpload::removeFile() {
  int index = fileList.indexOf(sendBlock(sender()));
  QString filename = fileList[index]->text;

  QString dir = getSetting("uploads/dir").toString() + QDir::separator();
  QFile file(QString(dir) + filename);

  if (file.remove())
    qDebug() << "Файл " << filename << " удален";
  else
    qDebug() << "Файл " << filename << " не доступен!";

  fileList[index]->hide();
  delete fileList[index];
  fileList.remove(index);

  if (!fileList.size())
    fileListBlock->hide();
  else
    fileListBlock->setFixedHeight(fileList.size() * 63 + 30);
}

// Удаление файла по имени
void FileUpload::removeFile(QString fileName) {
  int index = searchFileByName(fileName);
  if (index != -1) {
    QString dir = getSetting("uploads/dir").toString() + QDir::separator();
    QFile file(QString(dir) + fileName);

    if (file.remove())
      qDebug() << "Файл " << fileName << " удален";
    else
      qDebug() << "Файл " << fileName << " не доступен!";

    fileList[index]->hide();
    delete fileList[index];
    fileList.remove(index);

    if (!fileList.size())
      fileListBlock->hide();
    else
      fileListBlock->setFixedHeight(fileList.size() * 63 + 30);
  }
}

// Обновление списка файлов
void FileUpload::refreshList() {

  // Очистка списка
  for (auto fileBlock : fileList) {
    fileBlock->hide();
    delete fileBlock;
  }
  fileList.clear();

  // Добавление файлов из папки
  QString dirName = getSetting("uploads/dir").toString();
  QDir dir(dirName);

  foreach (QString file, dir.entryList(QDir::Files)) {
    QFileInfo fileInfo(file);
    QString filename = fileInfo.fileName();

    FileBlock *block = new FileBlock(filename, this);

    connect(block, SIGNAL(remove()), this, SLOT(removeFile()));
    fileList.push_back(block);
    fileListBlock->layout()->addWidget(block);
  }

  // Настройка высоты блока
  if (fileList.size()) {
    fileListBlock->show();
    fileListBlock->setFixedHeight(fileList.size() * 63 + 30);
  }

  QMessageBox::information(this, "Обновление", "Список файлов успешно обновлен");
  qDebug() << "Список обновлен";
}

// Вызов окна загрузки файлов
void FileUpload::callWeb() {
  if (!web) {
    dwnMenu->setStyleSheet("color: #f54545");

    page = new WebLoader(this);
    page->show();
    page->filesUpload();
    web = true;

    connect(page, SIGNAL(shutdown()), this, SLOT(noWebDialog()));
    connect(page, SIGNAL(fileDelRequest(QString)), this, SLOT(removeFile(QString)));
    connect(page, SIGNAL(fileAddRequest(QString)), this, SLOT(addFile(QString)));
  }
}

// Обновление состояния диалогового окна загрузки
void FileUpload::noWebDialog() {
  dwnMenu->setStyleSheet("color: black");
  web = false;
}
