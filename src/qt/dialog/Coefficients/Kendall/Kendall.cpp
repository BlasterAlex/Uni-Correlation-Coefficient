#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QTextStream>
#include <QVector>

#include "../../../Table/Table.hpp"
#include "../../../settings/settings.hpp"
#include "../Coefficients.hpp"

// Вычисление коэффициентов Кендалла
float Coefficients::getKendallCoeff(int firstI, int secondI) {

  QMultiMap<float, float> rows; // для хранения двух последовательностей

  Table first = tables[firstI], second = tables[secondI];
  int n = first.data.size();

  QString path = getSetting("output/coeff").toString() + QDir::separator() + "Kendall";
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

  QFile file(path + name);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream stream(&file);

    // Сортировка последовательностей по первому значению
    for (int i = 0; i < n; i++) {
      float a = first.data[i][0].toFloat();
      float b = second.data[i][0].toFloat();
      rows.insert(a, b);
    }
    stream << "x" << separator << "y" << separator << tr("Совп.") << separator << tr("Измен.") << endl;

    float P = 0, Q = 0;
    for (auto it = rows.begin(); it != rows.end(); ++it) {
      float matches = 0, inversion = 0;
      for (auto curIt = it; curIt != rows.end(); ++curIt) {
        if (curIt.value() > it.value())
          matches++;
        else if (curIt.value() < it.value())
          inversion++;
      }
      P += matches;
      Q += inversion;
      stream << it.key() << separator << it.value() << separator << matches << separator << inversion << endl;
    }

    stream << endl;
    stream << "P = " << separator << P << separator << " Q = " << separator << Q << endl;
    float coeff = 2 * (P - Q) / (n * (n - 1));
    stream << "coeff = " << separator << coeff << endl;

    file.close();
    return coeff;
  }

  return -1;
}