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
  explicit Chart(QWidget *parent = 0);
  virtual ~Chart() {}

protected:
  void closeEvent(QCloseEvent *); // закрытие окна

signals:
  void shutdown();
};

#endif // CHART_H