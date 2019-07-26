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

#include "../../Table/Table.hpp"

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView {
  Q_OBJECT

public:
  explicit ChartView(QVector<Table> &, QWidget *parent = 0);
  virtual ~ChartView() {}
};

#endif // CHARTVIEW_H