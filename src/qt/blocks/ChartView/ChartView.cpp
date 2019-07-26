/***
 * Copyright 2019 Alexander Pishchulev (https://github.com/BlasterAlex)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#include "../../Table/Table.hpp"
#include "ChartView.hpp"

ChartView::ChartView(QVector<Table> &tables, QWidget *parent) : QChartView(new QChart(), parent) {

  // Определение масштаба графика
  int tableSize = tables[0].getSize();
  float maxRank = std::max(tables[0].maxRank, tables[1].maxRank);
  int range = ceil(maxRank * 1.05);

  // Точки на плоскости
  QScatterSeries *series1 = new QScatterSeries();
  series1->setName("scatter1");
  series1->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  series1->setMarkerSize(10.0);

  // Добавление точек на плоскость
  for (int i = 0; i < tableSize; ++i)
    series1->append(tables[0].data[i][0].toFloat(), tables[1].data[i][0].toFloat());

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