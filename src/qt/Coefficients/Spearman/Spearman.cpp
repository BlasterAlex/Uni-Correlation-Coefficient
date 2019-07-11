#include "../../Table/Table.hpp"
#include "../Coefficients.hpp"

float Coefficients::getSpearmanCoeff(int firstI, int secondI) {

  float coeff = 0, a, b;
  Table first = tables[firstI], second = tables[secondI];

  for (int i = 0; i < first.data.size(); i++) {
    a = first.data[i][0].toFloat();
    b = second.data[i][0].toFloat();
    a -= b;
    coeff += a * a;
  }

  coeff = 6 / (coeff * (coeff - 1) * (coeff + 1));
  return coeff;
}