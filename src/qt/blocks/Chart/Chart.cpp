#include <QCloseEvent>
#include <QDialog>
#include <QLabel>
#include <QScatterSeries>
#include <QString>
#include <QTableWidget>
#include <QVector>
#include <QWidget>
#include <QtCharts>

#include "../../Table/Table.hpp"
#include "Chart.hpp"
#include "ChartView/ChartView.hpp"

Chart::Chart(QWidget *parent) : QDialog(parent) {

  setMinimumHeight(500);
  setMinimumWidth(700);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(new ChartView(this));
}

// Событие закрытия окна
void Chart::closeEvent(QCloseEvent *event) {
  emit shutdown();
  event->accept();
}