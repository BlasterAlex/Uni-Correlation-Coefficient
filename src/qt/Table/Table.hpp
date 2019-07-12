#ifndef TABLE_H
#define TABLE_H

#include <QString>
#include <QVector>

class Table {
  QString name;                   // название файла
  QVector<QVector<QString>> data; // таблица

public:
  explicit Table() {}
  explicit Table(QString filename);
  ~Table() {}

  int search(QString);        // поиск в таблице
  void writeToCSV();          // запись таблицы в файл
  void analysisWith(Table &); // сравнение с другой таблицей
  int getSize();              // получение размера таблицы

  friend class Coefficients;
  friend class TableDialog;
  friend class ChartView;
};

#endif // TABLE_H