#include <QChartView>
#include <QCloseEvent>
#include <QDialog>
#include <QLabel>
#include <QScatterSeries>
#include <QString>
#include <QTableWidget>
#include <QValueAxis>
#include <QVector>
#include <QWidget>
#include <QtCharts>

#include "../../../Table/Table.hpp"
#include "ChartView.hpp"

ChartView::ChartView(QWidget *parent) : QChartView(new QChart(), parent) {

  // Точки на плоскости
  QScatterSeries *series = new QScatterSeries();
  series->setName("scatter1");
  series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  series->setMarkerSize(10.0);

  // Добавление точек на плоскость
  series->append(0, 6);
  series->append(2, 4);
  series->append(3, 8);
  series->append(7, 4);
  series->append(10, 5);

  // Сглаживание
  setRenderHint(QPainter::Antialiasing);
  chart()->addSeries(series);
  chart()->setTitle("Корреляция");

  // Отключение тени
  chart()->setDropShadowEnabled(false);
  // Отключение легенды
  chart()->legend()->hide();

  // Настройка осей графика
  QValueAxis *axisX = new QValueAxis();
  QValueAxis *axisY = new QValueAxis();

  axisX->setRange(0, 300);
  axisX->setTitleText("First");

  axisY->setRange(0, 300);
  axisY->setTitleText("Second");

  chart()->addAxis(axisX, Qt::AlignBottom);
  chart()->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisX);
  series->attachAxis(axisY);
}