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
#include <QHeaderView>
#include <QLayout>
#include <QList>
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

#include "../../blocks/Chart/Chart.hpp"
#include "../../blocks/TableDialog/TableDialog.hpp"
#include "../Coefficients.hpp"

#define sendAction qobject_cast<QToolButton *>

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
  act1->setText("&Exchange");
  act1->setStyleSheet("color: black");
  act1->setFocusPolicy(Qt::NoFocus);
  act1->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
  toolBar->addWidget(act1);
  connect(act1, SIGNAL(clicked()), this, SLOT(action()));

  QToolButton *act2 = new QToolButton(toolBar);
  act2->setText("&Kendall");
  act2->setStyleSheet("color: black");
  act2->setFocusPolicy(Qt::NoFocus);
  act2->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));
  toolBar->addWidget(act2);
  connect(act2, SIGNAL(clicked()), this, SLOT(action()));

  QToolButton *act3 = new QToolButton(toolBar);
  act3->setText("&Spearman");
  act3->setStyleSheet("color: #f54545;");
  act3->setFocusPolicy(Qt::NoFocus);
  act3->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
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
                              "  min-width: 690px;"
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
                              "QHeaderView::section::hover {"
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
  QFormLayout *vbox = new QFormLayout(contentBlock);
  vbox->setMargin(0);
  vbox->setContentsMargins(5, 5, 10, 5);

  // Заголовок
  QLabel *textLabel = new QLabel(contentBlock);
  textLabel->setText("Коэффициенты корреляции Спирмана");
  QFont font = textLabel->font();
  font.setPointSize(10);
  font.setBold(true);
  textLabel->setFont(font);

  vbox->addWidget(textLabel);

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

  table->setRowCount(tablesNum);
  table->setColumnCount(tablesNum);

  QStringList headers;
  foreach (Table table, tables)
    headers << table.name.split(".")[0];

  table->setHorizontalHeaderLabels(headers);
  table->setVerticalHeaderLabels(headers);

  vbox->addWidget(table);

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
  actions << "&Exchange"
          << "&Kendall"
          << "&Spearman";

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
    contentBlock->findChild<QLabel *>()->setText("Коэффициенты корреляции Спирмана");
    newDataTable(Spearman);
    break;
  }
}

// Нажатие на ячейку таблицы
void Coefficients::cellSelected(int nRow, int nCol) {
  if (nRow != nCol) {
    qDebug() << "Row: " << QString::number(nRow) << " Col: " << QString::number(nCol);
    graph = new Chart(this);
    graph->show();
  }
}

// Нажатие на заголовок таблицы
void Coefficients::headerSelected(int num) {
  tableDialog = new TableDialog(tables[num], this);
  tableDialog->show();
  // dialog = true;
  // connect(tableDialog, SIGNAL(shutdown()), this, SLOT(noDialog()));
}