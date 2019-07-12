#include <QChartView>
#include <QCloseEvent>
#include <QDialog>
#include <QFont>
#include <QLabel>
#include <QLineSeries>
#include <QPen>
#include <QScatterSeries>
#include <QString>
#include <QTableWidget>
#include <QValueAxis>
#include <QVector>
#include <QWidget>
#include <QtCharts>

#include "../../../Table/Table.hpp"
#include "ChartView.hpp"

ChartView::ChartView(QVector<Table> &tables, QWidget *parent) : QChartView(new QChart(), parent) {

  // Определение масштаба графика
  int tableSize = tables[0].getSize();
  float maxRank = std::max(tables[0].data[tableSize - 1][0].toFloat(), tables[1].data[tableSize - 1][0].toFloat());
  int range = ceil(maxRank * 1.05);

  // Сортировка таблиц по названиям университетов
  QCollator collator;
  collator.setNumericMode(true);

  std::sort(tables[0].data.begin(), tables[0].data.end(),
            [&collator](const QVector<QString> &a, const QVector<QString> &b) { return collator.compare(a[1], b[1]) < 0; });
  std::sort(tables[1].data.begin(), tables[1].data.end(),
            [&collator](const QVector<QString> &a, const QVector<QString> &b) { return collator.compare(a[1], b[1]) < 0; });

  // Точки на плоскости
  QScatterSeries *series1 = new QScatterSeries();
  series1->setName("scatter1");
  series1->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  series1->setMarkerSize(10.0);

  // Добавление точек на плоскость
  for (int i = 0; i < tableSize; ++i) {
    series1->append(tables[0].data[i][0].toFloat(), tables[1].data[i][0].toFloat());
  }

  // Прямая y = x
  QLineSeries *series2 = new QLineSeries();
  series2->append(0, 0);
  series2->append(range, range);
  QPen pen = series2->pen();
  pen.setColor("red");
  series2->setPen(pen);

  // Сглаживание
  setRenderHint(QPainter::Antialiasing);

  // Добавление графиков
  chart()->addSeries(series1);
  chart()->addSeries(series2);

  // Название
  QFont font;
  font.setPointSize(13);
  font.setBold(true);
  chart()->setTitleFont(font);
  chart()->setTitle("Диаграмма рассеяния");

  // Отключение тени
  chart()->setDropShadowEnabled(false);
  // Отключение легенды
  chart()->legend()->hide();

  // Настройка осей графика
  QValueAxis *axisX = new QValueAxis();
  axisX->setRange(0, range);
  axisX->setTitleText(tables[0].name.split(".")[0]);

  QValueAxis *axisY = new QValueAxis();
  axisY->setRange(0, range);
  axisY->setTitleText(tables[1].name.split(".")[0]);

  chart()->addAxis(axisX, Qt::AlignBottom);
  chart()->addAxis(axisY, Qt::AlignLeft);
  series1->attachAxis(axisX);
  series1->attachAxis(axisY);
  series2->attachAxis(axisX);
  series2->attachAxis(axisY);
}