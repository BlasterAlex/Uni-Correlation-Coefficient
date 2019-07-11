#include "fileParsing.hpp"
#include "../../Table/Table.hpp"
#include "../../settings/settings.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>

void parsingFiles(QVector<Table> &);
void writingFiles(QVector<Table> &);

// Чтение таблиц из файлов
QVector<Table> getParsedTables(QVector<QString> files) {
  QVector<Table> tables; // массив таблиц
  foreach (const QString &filename, files) {
    Table table(filename);
    tables.push_back(table);
  }
  parsingFiles(tables);
  return tables;
}

// Разбор таблиц
void parsingFiles(QVector<Table> &tables) {

  // Сравнение всех таблиц с первой
  for (int i = 1; i < tables.size(); ++i) {
    tables[0].analysisWith(tables[i]);
  }

  // Теперь первая таблица содержит только записи, которые содержатся во всех остальных таблицах

  // Сравнение первой таблицы со всеми
  for (int i = 1; i < tables.size(); ++i) {
    tables[i].analysisWith(tables[0]);
  }

  // Запись новых таблиц
  writingFiles(tables);
}

// Запись таблиц в файлы
void writingFiles(QVector<Table> &tables) {

  QString target = getSetting("uploads/changesDir").toString();
  QDir dir(target);
  if (!dir.exists()) {
    QDir().mkdir(target);
    qDebug() << "Создана папка " + target;
  }
  target += QDir::separator();

  foreach (Table table, tables) { table.writeToCSV(); }
}