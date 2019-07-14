#ifndef TABLE_H
#define TABLE_H

#include <QString>
#include <QVector>

class Table {
  QString name;                   // название файла
  QVector<QVector<QString>> data; // таблица
  float maxRank;                  // максимальное значение ранга в таблице

public:
  explicit Table() {}
  explicit Table(QString filename);
  ~Table() {}

  int search(QString);        // поиск в таблице
  void writeToCSV();          // запись таблицы в файл
  void analysisWith(Table &); // сравнение с другой таблицей
  void sortByName();          // сортировка таблицы по названиям
  int getSize();              // получение размера таблицы

  friend class Coefficients;
  friend class TableDialog;
  friend class ChartView;
};

#endif // TABLE_H