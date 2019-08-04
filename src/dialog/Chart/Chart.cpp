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

  // Название
  setWindowTitle("Диаграмма рассеяния");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(new ChartView(tables, this));
}

// Событие закрытия окна
void Chart::closeEvent(QCloseEvent *event) {
  emit shutdown();
  event->accept();
}