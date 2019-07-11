#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include <QCloseEvent>
#include <QDialog>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../../../Table/Table.hpp"

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView {
  Q_OBJECT

public:
  explicit ChartView(QWidget *parent = 0);
  virtual ~ChartView() {}
};

#endif // CHARTVIEW_H