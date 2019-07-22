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