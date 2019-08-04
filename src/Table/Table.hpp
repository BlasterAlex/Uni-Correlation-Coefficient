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

#ifndef TABLE_H
#define TABLE_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QVector>

class Table {
  QString name;                   // название файла
  QVector<QVector<QString>> data; // таблица
  float maxRank;                  // максимальное значение ранга в таблице

public:
  explicit Table() {}
  explicit Table(QString);
  explicit Table(QList<QList<QVariant>>, QString);
  ~Table() {}

  int search(QString);        // поиск в таблице
  void writeToCSV();          // запись таблицы в файл
  bool createNewFile();       // сохранение новой таблицы
  void analysisWith(Table &); // сравнение с другой таблицей
  void sortByName();          // сортировка таблицы по названиям
  int getSize();              // получение размера таблицы

  friend class Coefficients;
  friend class TableDialog;
  friend class ChartView;
};

#endif // TABLE_H