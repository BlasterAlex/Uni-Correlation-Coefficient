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

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>

#include "../../Table/Table.hpp"
#include "../../settings/settings.hpp"
#include "../FileUpload.hpp"

void parsingFiles(QVector<Table> &);
void writingFiles(QVector<Table> &);

// Чтение таблиц из файлов
QVector<Table> FileUpload::getParsedTables(QVector<QString> files) {
  QVector<Table> tables; // массив таблиц
  foreach (const QString &filename, files) {
    Table table(filename);

    if (table.getSize() == 0 || table.getSize() == 1) // пустая таблица
      return QVector<Table>();

    tables.push_back(table);
  }
  parsingFiles(tables);
  return tables;
}

// Разбор таблиц
void parsingFiles(QVector<Table> &tables) {

  // Сравнение всех таблиц с первой
  for (int i = 1; i < tables.size(); ++i)
    tables[0].analysisWith(tables[i]);

  /*** Теперь первая таблица содержит только записи,
   * которые содержатся во всех остальных таблицах
   */

  // Сравнение первой таблицы со всеми
  for (int i = 1; i < tables.size(); ++i)
    tables[i].analysisWith(tables[0]);

  // Сортировка таблиц по названиям университетов
  for (int i = 0; i < tables.size(); ++i)
    tables[i].sortByName();

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