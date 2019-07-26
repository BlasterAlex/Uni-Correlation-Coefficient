
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

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLayout>
#include <QList>
#include <QMessageBox>
#include <QMultiMap>
#include <QObject>
#include <QProgressBar>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "../../Chart/Chart.hpp"
#include "../../TableDialog/TableDialog.hpp"
#include "../Coefficients.hpp"

#define sendAction qobject_cast<QToolButton *>
#define sendTableDialog qobject_cast<TableDialog *>
#define sendGraphDialog qobject_cast<Chart *>

// Создание полоски загрузки
void Coefficients::createLoader() {
  progress = new QGroupBox(this);
  QVBoxLayout *vbox = new QVBoxLayout(progress);

  progress_bar = new QProgressBar(progress);
  progress_bar->setMinimum(1);
  progress_bar->setMaximum(iterationsCol(tablesNum - 1) * 3);
  vbox->addWidget(progress_bar);

  progress_label = new QLabel(progress);
  progress_label->setText("Вычисление коэффициентов...");
  vbox->addWidget(progress_label);
}

// Создание меню
void Coefficients::setMenu() {
  toolBar = new QToolBar(this);

  // Кнопки действий
  QToolButton *act1 = new QToolButton(toolBar);
  act1->setText("&Обмен");
  act1->setStyleSheet("color: black");
  act1->setFocusPolicy(Qt::NoFocus);
  act1->setShortcut(QKeySequence(Qt::ALT + Qt::Key_J));
  toolBar->addWidget(act1);
  connect(act1, &QToolButton::clicked, this, &Coefficients::action);

  QToolButton *act2 = new QToolButton(toolBar);
  act2->setText("&Кендалл");
  act2->setStyleSheet("color: black");
  act2->setFocusPolicy(Qt::NoFocus);
  act2->setShortcut(QKeySequence(Qt::ALT + Qt::Key_R));
  toolBar->addWidget(act2);
  connect(act2, &QToolButton::clicked, this, &Coefficients::action);

  QToolButton *act3 = new QToolButton(toolBar);
  act3->setText("&Спирмен");
  act3->setStyleSheet("color: #f54545");
  act3->setFocusPolicy(Qt::NoFocus);
  act3->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));
  toolBar->addWidget(act3);
  connect(act3, SIGNAL(clicked()), this, SLOT(action()));

  toolBar->setLayoutDirection(Qt::LeftToRight);
  toolBar->hide();
}

// Создание поля для вывода результатов
void Coefficients::createContentBlock() {
  contentBlock = new QGroupBox(this);
  contentBlock->setStyleSheet("QGroupBox {"
                              "  border: none;"
                              "  border-top: 2px solid rgba(178, 178, 178, 178);"
                              "  margin-top: 0;"
                              "  text-align: center;"
                              "  min-width: 700px;"
                              "}"
                              ""
                              "QTableView {"
                              "  selection-background-color: #c9d6ff;"
                              "}"
                              ""
                              "QTableWidget::item:hover {"
                              "  background-color: #e8f6ff;"
                              "  opacity: 0.8;"
                              "  text-decoration: underline;"
                              "  font-weight: bold;"
                              "}"
                              ""
                              "QHeaderView::section {"
                              "  background-color: #bdbdbd;"
                              "  padding: 4px;"
                              "  font-size: 14pt;"
                              "  border-style: none;"
                              "  border-bottom: 1px solid #fffff8;"
                              "  border-right: 1px solid #fffff8;"
                              "}"
                              ""
                              "QHeaderView::section:horizontal {"
                              "  border-top: 1px solid #fffff8;"
                              "}"
                              ""
                              "QHeaderView::section:vertical {"
                              "  border-left: 1px solid #fffff8;"
                              "  min-width: 100px;"
                              "}"
                              ""
                              "QHeaderView::section:horizontal:hover,"
                              "QHeaderView::section:vertical:hover {"
                              "  background-color: #a8a8a8;"
                              "  opacity: 0.8;"
                              "  text-decoration: underline;"
                              "  font-weight: bold;"
                              "}");

  // Слой
  QVBoxLayout *vbox = new QVBoxLayout(contentBlock);
  vbox->setMargin(0);
  vbox->setContentsMargins(5, 5, 10, 5);

  // Верхний слой
  QHBoxLayout *header = new QHBoxLayout;

  // Заголовок
  QLabel *textLabel = new QLabel(contentBlock);
  textLabel->setText("Коэффициенты корреляции Спирмена");
  QFont font = textLabel->font();
  font.setPointSize(10);
  font.setBold(true);
  textLabel->setFont(font);
  textLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  header->addWidget(textLabel);
  header->addStretch();

  // Количество университетов
  QLabel *uniLabel = new QLabel(contentBlock);
  uniLabel->setText("Университетов:");
  font = uniLabel->font();
  font.setPointSize(9);
  uniLabel->setFont(font);
  uniLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);

  header->addWidget(uniLabel);

  QLabel *uniCount = new QLabel(contentBlock);
  uniCount->setText(QString::number(tables[0].getSize()));
  font = uniCount->font();
  font.setPointSize(9);
  font.setBold(true);
  uniCount->setFont(font);
  uniCount->setAlignment(Qt::AlignTop | Qt::AlignRight);

  header->addWidget(uniCount);
  vbox->addLayout(header);

  // Таблица
  table = new QTableWidget(contentBlock);

  // Запрет редактирования таблицы
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  // Отключение фокуса на таблице
  table->setFocusPolicy(Qt::NoFocus);
  table->setSelectionMode(QAbstractItemView::NoSelection);
  // Размеры
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  contentBlock->setMinimumHeight(tables.size() * 50 + 30);

  table->setRowCount(tablesNum);
  table->setColumnCount(tablesNum);

  QStringList headers;
  foreach (Table table, tables)
    headers << table.name.split(".")[0];

  table->setHorizontalHeaderLabels(headers);
  table->setVerticalHeaderLabels(headers);

  vbox->addWidget(table, 1, Qt::AlignTop);

  connect(table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerSelected(int)));
  connect(table->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerSelected(int)));
  connect(table, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelected(int, int)));

  contentBlock->hide();
}

// Записать новую информацию в таблицу
void Coefficients::newDataTable(QVector<Coeff> coeffs) {
  foreach (Coeff coeff, coeffs) {
    QTableWidgetItem *element = new QTableWidgetItem(coeff.val);
    element->setTextAlignment(Qt::AlignCenter);
    table->setItem(coeff.parents[0], coeff.parents[1], element);
    table->setItem(coeff.parents[1], coeff.parents[0], element->clone());
  }
}

// Отображение результатов
void Coefficients::displayResults() {
  if (progress_bar->value() == iterationsCol(tablesNum - 1) * 3) {
    // Удалить полоску загрузки
    progress->hide();
    delete progress;

    // Отобразить меню
    toolBar->show();

    // Отображение контента
    contentBlock->show();
    newDataTable(Spearman);

  } else {
    qDebug() << "Вычисление еще не окончено!";
  }
}

// Нажатие кнопки меню
void Coefficients::action() {
  QStringList actions;
  actions << "&Обмен"
          << "&Кендалл"
          << "&Спирмен";

  // Сброс стилей для всех кнопок
  QList<QToolButton *> toolButtons = toolBar->findChildren<QToolButton *>();
  foreach (QToolButton *button, toolButtons) { button->setStyleSheet("color: black;"); }

  // Установка стиля для нажатой кнопки
  sendAction(sender())->setStyleSheet("color: #f54545;");

  switch (actions.indexOf(sendAction(sender())->text())) {
  case 0:
    contentBlock->findChild<QLabel *>()->setText("Коэффициенты корреляции, метод обмена");
    newDataTable(Exchange);
    break;
  case 1:
    contentBlock->findChild<QLabel *>()->setText("Коэффициенты корреляции Кендалла");
    newDataTable(Kendall);
    break;
  case 2:
    contentBlock->findChild<QLabel *>()->setText("Коэффициенты корреляции Спирмена");
    newDataTable(Spearman);
    break;
  }
}

// Нажатие на заголовок таблицы
void Coefficients::headerSelected(int num) {
  if (openedTables.indexOf(num) == -1) { // открыть диалоговое окно таблицы, если она еще не открыта
    tableDialog = new TableDialog(num, tables[num], this);
    tableDialog->show();

    connect(tableDialog, SIGNAL(shutdown()), this, SLOT(minusDialog()));
    openedTables.push_back(num);
  }
}
// Закрытие диалогового окна таблицы
void Coefficients::minusDialog() {
  int num = sendTableDialog(sender())->num;
  openedTables.remove(openedTables.indexOf(num)); // удалить окно из списка открытых
}

// Нажатие на ячейку таблицы
void Coefficients::cellSelected(int nRow, int nCol) {
  if (nRow != nCol) {
    // Для проверки на дублирование окна
    QString cord = QString::number(nRow) + QString::number(nCol);
    QString rCord;
    for (int i = cord.size() - 1; i >= 0; --i)
      rCord += cord[i];

    // Если такое окно не открыто
    if (openedGraphs.indexOf(cord) == -1 && openedGraphs.indexOf(rCord) == -1) {
      graph = new Chart(cord, QVector<Table>({tables[nCol], tables[nRow]}), this);
      graph->show();

      connect(graph, SIGNAL(shutdown()), this, SLOT(minusGraph()));
      openedGraphs.push_back(cord);
    }
  }
}
// Закрытие диалогового графика
void Coefficients::minusGraph() {
  QString cord = sendGraphDialog(sender())->cord;
  QString rCord;
  for (int i = cord.size() - 1; i >= 0; --i)
    rCord += cord[i];

  // Поиск координаты в массиве
  int index = openedGraphs.indexOf(cord);
  if (index == -1)
    index = openedGraphs.indexOf(rCord);

  openedGraphs.remove(index); // удалить окно из списка открытых
}