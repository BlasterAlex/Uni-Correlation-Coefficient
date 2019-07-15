#ifndef CHART_H
#define CHART_H

#include <QCloseEvent>
#include <QDialog>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../../Table/Table.hpp"

class Chart : public QDialog {
  Q_OBJECT

public:
  explicit Chart(QString, QVector<Table>, QWidget *parent = 0);
  virtual ~Chart() {}

protected:
  void closeEvent(QCloseEvent *); // закрытие окна

private:
  QString cord; // номера таблиц в списке

signals:
  void shutdown();

  friend class Coefficients;
};

#endif // CHART_H