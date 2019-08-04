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

#include <QCollator>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QList>
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <QVariant>
#include <QVector>

#include "../settings/settings.hpp"
#include "Table.hpp"

#define Iterator QMutableVectorIterator<QVector<QString>>

// Среднее значение диапазона последовательности элементов
float rangeAverage(int first, int last) {
  float sum = 0;
  float count = 0;

  // Нужны номера, а не индексы
  ++first;
  ++last;

  while (first < last) {
    sum += first;
    ++first;
    ++count;
  }

  return sum / count;
}

// Чтение таблицы из файла
Table::Table(QString filename) : name(filename) {

  QString dir = getSetting("uploads/dir").toString() + QDir::separator();
  QFile file(dir + name);
  QString separator = getSetting("uploads/csvSeparator").toString();

  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    qDebug() << "Файл " + name + " недоступен!";
  } else {
    // Поток для извлечения данных из файла
    QTextStream in(&file);

    // Одна таблица
    QVector<QVector<QString>> table;

    // Чтение файла до конца
    while (!in.atEnd()) {
      QString line = in.readLine();
      QVector<QString> tableLine;

      for (QString item : line.split(separator))
        tableLine.push_back(item);

      table.push_back(tableLine);
    }
    file.close();

    // Сохранение новой таблицы
    data = table;

    // Сохранение максимального значения ранга
    maxRank = data[data.size() - 1][0].toFloat();
  }
}

// Чтение таблицы из двумерного списка
Table::Table(QList<QList<QVariant>> list, QString filename) : name(filename) {
  QVector<QVector<QString>> table;

  foreach (QList<QVariant> row, list) {
    QVector<QString> tableLine;

    tableLine.push_back(row[0].toString());
    tableLine.push_back(row[1].toString());

    table.push_back(tableLine);
  }

  // Сохранение новой таблицы
  data = table;

  // Сохранение максимального значения ранга
  maxRank = data[data.size() - 1][0].toFloat();
}

// Поиск в таблице
int Table::search(QString keyName) {
  int index = 0;
  foreach (QVector<QString> tableLine, data) {
    if (tableLine[1].compare(keyName) == 0)
      return index;
    ++index;
  }
  return -1;
}

// Запись таблицы в файл
void Table::writeToCSV() {
  QString path = getSetting("uploads/changesDir").toString() + QDir::separator();
  QString separator = getSetting("uploads/csvSeparator").toString();

  QFile file(path + name);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream stream(&file);
    foreach (QVector<QString> tableLine, data)
      stream << tableLine[0] << separator << tableLine[1] << endl;
    file.close();
  }
}

// Сохранение новой таблицы
bool Table::createNewFile() {
  QString path = getSetting("uploads/dir").toString() + QDir::separator();
  QString separator = getSetting("uploads/csvSeparator").toString();

  QFile file(path + name);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream stream(&file);
    foreach (QVector<QString> tableLine, data)
      stream << tableLine[0] << separator << tableLine[1] << endl;
    file.close();
    return true;
  } else
    return false;
}

// Сравнение с другой таблицей
void Table::analysisWith(Table &another) {

  /*** Алгоритм:
   * Производится проход по таблице с поиском текущего названия в другой таблице.
   * При нахождении несовпадения, запись из первой таблицы удаляется.
   * Последовательность одинаковых рангов заменяется средним арифметическим
   * номеров соответсвующих элементов в таблице
   */

  int index = 0;       // текущий индекс элемента
  int begin = 0;       // индекс начала диапазона одинаковых рангов
  float prevRank = -1; // ранг предыдущего университета

  Iterator iter(this->data);
  while (iter.hasNext()) {

    int res = another.search(iter.next()[1]); // поиск в другой таблице
    float rank = iter.value()[0].toFloat();   // получение значения текущего ранга

    if (res != -1) { // совпадение - не думаю

      // Если предыдущий ранг отличен от текущего
      if (prevRank != rank) {

        // Если есть диапазон одинаковых рангов
        if (begin != index - 1) {
          // Среднее арифметическое диапазона
          float average = rangeAverage(begin, index);

          // Обновить значения диапазона
          for (int i = begin; i < index; ++i)
            data[i][0] = QString::number(average);
        }

        // Значение текущего ранга
        iter.value()[0] = QString::number(index + 1);
        begin = index; // запомнить индекс текущего элемента
      }

      ++index;         // увеличение индекса
      prevRank = rank; // запоминание текущего старого значения текущего ранга

    } else // различие - просто удалить
      iter.remove();
  }

  // Если остался диапазон после прохода
  if (begin != data.size() - 1) {
    // Среднее арифметическое диапазона
    float average = rangeAverage(begin, data.size());

    // Обновить значения диапазона
    for (int i = begin; i < data.size(); ++i)
      data[i][0] = QString::number(average);
  }

  // Обновление максимального значения ранга
  maxRank = data[data.size() - 1][0].toFloat();
}

// Сортировка таблицы по названиям
void Table::sortByName() {
  QCollator collator;
  collator.setNumericMode(true);
  std::sort(data.begin(), data.end(), [&collator](const QVector<QString> &a, const QVector<QString> &b) { return collator.compare(a[1], b[1]) < 0; });
}

// Получение размера таблицы
int Table::getSize() { return data.size(); }