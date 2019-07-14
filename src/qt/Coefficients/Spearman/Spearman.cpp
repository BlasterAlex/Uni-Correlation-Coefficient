#include "../../Table/Table.hpp"
#include "../Coefficients.hpp"

#include <QString>
#include <QVector>

float Coefficients::getSpearmanCoeff(int firstI, int secondI) {

  float coeff = 0, d;
  Table first = tables[firstI], second = tables[secondI];
  int size = first.getSize();

  for (int i = 0; i < size; ++i) {
    d = first.data[i][2].toInt() - second.data[i][2].toInt();
    coeff += d * d; // сумма квадратов разности рангов
  }

  coeff = (6 * coeff) / (size * (size - 1) * (size + 1));
  return coeff;
}