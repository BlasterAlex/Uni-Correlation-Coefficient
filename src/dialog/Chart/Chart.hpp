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