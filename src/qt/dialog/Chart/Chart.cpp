#include <QCloseEvent>
#include <QCollator>
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QScatterSeries>
#include <QString>
#include <QTableWidget>
#include <QVector>
#include <QWidget>
#include <QtAlgorithms>
#include <QtCharts>

#include "../../Table/Table.hpp"
#include "../../blocks/ChartView/ChartView.hpp"
#include "Chart.hpp"

Chart::Chart(QString n, QVector<Table> tables, QWidget *parent) : QDialog(parent) {
  cord = n;

  setMinimumHeight(700);
  setMinimumWidth(700);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(new ChartView(tables, this));
}

// Событие закрытия окна
void Chart::closeEvent(QCloseEvent *event) {
  emit shutdown();
  event->accept();
}