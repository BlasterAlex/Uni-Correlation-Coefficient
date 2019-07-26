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
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <iostream>

#include "../../../Table/Table.hpp"
#include "../../../settings/settings.hpp"
#include "../Coefficients.hpp"

// Вычисление коэффициентов методом обмена
float Coefficients::getExchange(int firstI, int secondI) {

  Table first = tables[firstI], second = tables[secondI];
  QMultiMap<float, float> rows; // для хранения двух последовательностей
  QVector<float> sequence;      // несортированная последовательность

  QString path = getSetting("output/coeff").toString() + QDir::separator() + "Exchange";
  QString separator = getSetting("uploads/csvSeparator").toString();

  // Имя выходного файла
  QString name = "tables_" + QString::number(firstI) + "&" + QString::number(secondI) + ".csv";

  // Создание папки для вывода, если нужно
  QDir dir(path);
  if (!dir.exists()) {
    QDir().mkdir(path);
    qDebug() << "Создана папка " + path;
  }
  path += QDir::separator();

  // Заполнение последовательности
  for (int i = 0; i < first.data.size(); i++) {
    float a = first.data[i][0].toFloat();
    float b = second.data[i][0].toFloat();
    rows.insert(a, b);
  }

  for (auto it = rows.begin(); it != rows.end(); ++it) // для сортировки
    sequence.push_back(it.value());

  int n = sequence.size();
  float iterations = 0;
  for (int i = 1; i < n; i++) { // пузырьковая сортировка
    for (int r = 0; r < n - i; r++) {
      if (sequence[r] > sequence[r + 1]) { // обмен местами
        std::swap(sequence[r], sequence[r + 1]);
        iterations++;
      }
    }
  }

  QFile file(path + name);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream stream(&file);
    foreach (float n, sequence)
      stream << n << endl;
    file.close();
  }

  return 1 - iterations / (n * (n - 1) / 2);
}