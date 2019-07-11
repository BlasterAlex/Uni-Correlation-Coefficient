#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QFile>
#include <QFont>
#include <QFormLayout>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "../../Coefficients/Coefficients.hpp"
#include "../../Table/Table.hpp"
#include "../../blocks/FileBlock/FileBlock.hpp"
#include "../../blocks/HoverButton/HoverButton.hpp"
#include "../../blocks/UploadForm/UploadForm.hpp"
#include "../../settings/settings.hpp"
#include "../fileParsing/fileParsing.hpp"
#include "FileUpload.hpp"

#define sendBlock qobject_cast<FileBlock *>

FileUpload::FileUpload(QWidget *parent) : QWidget(parent) {

  // Основной слой
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setAlignment(Qt::AlignHCenter);

  // Создание поля загрузки файлов
  createDragAndDrop();
  mainLayout->addWidget(dragAndDrop);
  mainLayout->setAlignment(dragAndDrop, Qt::AlignHCenter);

  // Создание списка загруженных файлов
  createFileList();
  mainLayout->addWidget(fileListBlock);

  // Кнопка
  HoverButton *button = new HoverButton("Отправить", this);
  button->setStyleSheet("margin-top: 10px; width: 120px; height: 25px;");
  mainLayout->addWidget(button);
  mainLayout->setAlignment(button, Qt::AlignHCenter);
  connect(button, SIGNAL(clicked()), this, SLOT(submit()));
}

void FileUpload::createDragAndDrop() { // создание поля загрузки файлов
  dragAndDrop = new UploadForm(this);
  connect(dragAndDrop, SIGNAL(submit()), this, SLOT(formSubmited()));
}

// Вывод списка загруженных файлов
void FileUpload::createFileList() {
  fileListBlock = new QGroupBox(tr("Загруженные файлы"), this);
  fileListBlock->setMinimumWidth(300);

  QFormLayout *vbox = new QFormLayout(fileListBlock);

  QString dirName = getSetting("uploads/dir").toString();
  QDir dir(dirName);
  if (!dir.exists()) {
    QDir().mkdir(dirName);
    qDebug() << "Создана папка " + dirName;
  }

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
  else
    checkExtensions(); // проверка, если есть файлы
}

// Проверка файлов на тип
bool FileUpload::checkExtensions() {

  QStringList exts = getSetting("uploads/extensions").toStringList();

  foreach (FileBlock *fileBlock, fileList) {
    QString fileName = fileBlock->text;
    QString fileExt = QFileInfo(fileName).completeSuffix();

    if (exts.indexOf(fileExt) == -1) { // если это неподдерживаемый тип
      QMessageBox::StandardButton reply;
      reply = QMessageBox::critical(this, "Ошибка расширения", "Файл '" + fileName + "' имеет недопустимое расширение!\nМожно удалить этот файл?",
                                    QMessageBox::Yes | QMessageBox::No);
      if (reply == QMessageBox::Yes)
        removeFile(fileName); // удаление файла
      else
        return false;
    }
  }
  return true;
}

// Файлы с формы
void FileUpload::formSubmited() {
  // Копирование файлов в папку
  QString target = getSetting("uploads/dir").toString() + QDir::separator();

  if (QFile::exists(target))
    foreach (const QString &file, dragAndDrop->files) {
      QFileInfo fileInfo(file);
      QString filename = fileInfo.fileName();

      // Загружаемый файл существует в папке загрузок
      if (QFile::exists(target + filename)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, "Ошибка загрузки", "Файл '" + filename + "' уже существует!\nМожно переписать этот файл?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
          removeFile(filename); // удаление файла
      }

      if (QFile::copy(file, target + filename)) {
        qDebug() << "Файл " << filename << " загружен";
        addFile(file);
      } else
        qDebug() << "Файл " << filename << " не загружен!";
    }

  if (fileList.size())
    fileListBlock->show();
}

// Добавление нового файла
void FileUpload::addFile(QString file) {
  QFileInfo fileInfo(file);
  QString filename = fileInfo.fileName();
  FileBlock *block = new FileBlock(filename, this);

  connect(block, SIGNAL(remove()), this, SLOT(removeFile()));
  fileList.push_back(block);
  fileListBlock->layout()->addWidget(block);
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
};

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

// Отправка данных
void FileUpload::submit() {
  if (!dialog && checkExtensions()) { // если все файлы допустимого типа

    QVector<QString> files;
    foreach (FileBlock *fileBlock, fileList) { files.push_back(fileBlock->text); }

    // Массив таблиц
    QVector<Table> tables = getParsedTables(files);

    // Проверка количества таблиц
    if (tables.size() < 2) {
      QMessageBox::critical(this, "Ошибка", "Вы должны загрузить хотя бы две таблицы!");
    } else {

      // Проверка размера таблиц
      bool check = true;
      int size = tables[0].getSize();
      for (int i = 1; i < tables.size(); ++i)
        if (size != tables[i].getSize()) {
          check = false;
          break;
        }
      qDebug() << "Размер таблиц: " << size;

      // Вызов диалогового окна
      if (!check)
        QMessageBox::critical(this, "Ошибка", "Размеры таблиц не равны!");
      else {
        // Вычисление коэффициентов
        coeff = new Coefficients(tables, this);
        coeff->show();
        dialog = true;
        connect(coeff, SIGNAL(shutdown()), this, SLOT(noDialog()));
      }
    }
  }
};

// Обнуление счетчика открытых окон
void FileUpload::noDialog() { dialog = false; }