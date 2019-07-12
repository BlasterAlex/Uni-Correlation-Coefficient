#ifndef COEFF_H
#define COEFF_H

#include <QCloseEvent>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include <QTableWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVector>
#include <QWidget>

#include "../Table/Table.hpp"
#include "../blocks/Chart/Chart.hpp"
#include "../blocks/TableDialog/TableDialog.hpp"

struct Coeff {
  QVector<int> parents; // номера таблиц, к которым относится коэффициент
  QString val;          // вычисленный коэффициент

  Coeff() {}
  Coeff(int a, int b, QString value) {
    parents.push_back(a);
    parents.push_back(b);
    val = value;
  }
};

class Coefficients : public QDialog {
  Q_OBJECT

public:
  explicit Coefficients(QVector<Table>, QWidget *parent = 0);
  virtual ~Coefficients() {}

protected:
  void closeEvent(QCloseEvent *); // закрытие окна

private slots:
  void action();               // нажатие кнопки меню
  void cellSelected(int, int); // нажатие на ячейку таблицы
  void headerSelected(int);    // нажатие на заголовок таблицы
  void minusDialog();          // закрытие диалогового окна таблицы
  void minusGraph();           // закрытие диалогового графика

private:
  // Таблицы рейтингов
  QVector<Table> tables;
  int tablesNum;

  // Коэффициенты корреляции
  QVector<Coeff> Spearman;
  QVector<Coeff> Kendall;
  QVector<Coeff> Exchange;

  // Меню
  QToolBar *toolBar;

  // Полоска загрузки
  QGroupBox *progress;
  QLabel *progress_label;
  QProgressBar *progress_bar;

  // Контент
  QGroupBox *contentBlock;
  QTableWidget *table;

  // Диалоговое окно таблицы
  TableDialog *tableDialog;
  QVector<int> openedTables;

  // Диалоговое окно графика
  Chart *graph;
  QVector<QString> openedGraphs;

  void setMenu();            // создание меню
  void createContentBlock(); // создание поля для вывода результатов
  void createLoader();       // создание полоски загрузки

  int iterationsCol(int);                   // подсчет количества итераций алгоритмов
  void calculation();                       // вычисление коэффициентов
  float getSpearmanCoeff(int, int);         // вычисление коэффициентов Спирмана
  float getKendallCoeff(int, int);          // вычисление коэффициентов Кендалла
  float getExchange(int, int);              // вычисление коэффициентов методом обмена
  void writeToCSV(QVector<Coeff>, QString); // вывод результатов в файл
  void displayResults();                    // отображение результатов
  void newDataTable(QVector<Coeff>);        // записать новую информацию в таблицу

signals:
  void shutdown();
};

#endif // COEFF_H