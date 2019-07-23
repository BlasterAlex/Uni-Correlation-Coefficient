#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QFile>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "../Table/Table.hpp"
#include "../blocks/FileBlock/FileBlock.hpp"
#include "../blocks/HoverButton/HoverButton.hpp"
#include "../blocks/UploadForm/UploadForm.hpp"
#include "../dialog/Coefficients/Coefficients.hpp"
#include "../settings/settings.hpp"
#include "FileUpload.hpp"

FileUpload::FileUpload(QWidget *parent) : QMainWindow(parent) {

  // Главный виджет
  QWidget *central = new QWidget(this);

  // Основной слой
  QVBoxLayout *mainLayout = new QVBoxLayout(central);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);
  mainLayout->setAlignment(Qt::AlignHCenter);
  setMinimumWidth(455);

  // Создание меню
  createMenu();
  mainLayout->addWidget(menu, 0, Qt::AlignTop);

  // Создание поля загрузки файлов
  createDragAndDrop();
  mainLayout->addWidget(dragAndDrop, 0, Qt::AlignTop | Qt::AlignHCenter);

  // Создание списка загруженных файлов
  createFileList();
  mainLayout->addWidget(fileListBlock, 1, Qt::AlignTop);

  // Кнопка
  HoverButton *button = new HoverButton("Отправить", this);
  button->setStyleSheet("margin: 15px 0; width: 120px; height: 25px;");
  mainLayout->addWidget(button, 0, Qt::AlignTop | Qt::AlignHCenter);
  connect(button, &HoverButton::clicked, this, &FileUpload::submit);

  setCentralWidget(central);
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
        reply = QMessageBox::question(this, "Файл существует", "Файл '" + filename + "' уже существует!\nМожно переписать этот файл?",
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

  if (fileList.size()) {
    fileListBlock->show();
    fileListBlock->setFixedHeight(fileList.size() * 63 + 10);
  }
}

// Отправка данных
void FileUpload::submit() {
  if (!dialog && checkExtensions()) { // если все файлы допустимого типа

    QVector<QString> files;
    foreach (FileBlock *fileBlock, fileList) { files.push_back(fileBlock->text); }

    // Массив таблиц
    QVector<Table> tables = getParsedTables(files);

    // Проверка количества таблиц
    if (tables.size() == 0) {
      QMessageBox::critical(this, "Ошибка", "В списке есть пустая таблица!\nВычисление коэффициентов корреляции невозможно");
    } else if (tables.size() == 1) {
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
}

// Обновление состояния диалогового окна загрузки
void FileUpload::noDialog() { dialog = false; }

// Событие закрытия окна
void FileUpload::closeEvent(QCloseEvent *event) {
  if (web && !page->completenessCheck()) { // если идет скачивание таблиц
    event->ignore();                       // запретить выход
  } else
    event->accept();
}