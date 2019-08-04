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

#include <QString>
#include <QVector>

#include "../../../Table/Table.hpp"
#include "../Coefficients.hpp"

float Coefficients::getSpearmanCoeff(int firstI, int secondI) {

  float coeff = 0, d;
  Table first = tables[firstI], second = tables[secondI];
  int size = first.getSize();

  for (int i = 0; i < size; ++i) {
    d = first.data[i][0].toFloat() - second.data[i][0].toFloat();
    coeff += d * d; // сумма квадратов разности рангов
  }

  coeff = 1 - (6 * coeff / (size * (size - 1) * (size + 1)));
  return coeff;
}