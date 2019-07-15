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