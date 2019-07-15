#include <QCloseEvent>
#include <QDialog>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QVariant>
#include <QVector>
#include <QWidget>

#include "../../Table/Table.hpp"
#include "FloatItem/FloatItem.hpp"
#include "TableDialog.hpp"

TableDialog::TableDialog(int n, Table t, QWidget *parent) : QDialog(parent) {
  num = n;

  setMinimumWidth(550);
  setMinimumHeight(450);
  setStyleSheet("QHeaderView::section {"
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
                "QHeaderView::section:horizontal:hover {"
                "  background-color: #a8a8a8;"
                "  text-decoration: underline;"
                "  font-weight: bold;"
                "}"
                ""
                "QHeaderView::down-arrow {"
                "  image: url(data/images/sort-down-solid.svg);"
                "  width: 12px;"
                "  margin: 0 10px 0 0;"
                "}"
                ""
                "QHeaderView::up-arrow {"
                "  image: url(data/images/sort-up-solid.svg);"
                "  width: 12px;"
                "  margin: 0 10px 0 0;"
                "}"
                ""
                "QHeaderView::up-arrow,"
                "QHeaderView::down-arrow:hover {"
                "  width: 13px;"
                "}"
                ""
                "QLabel {"
                "  margin: 0 0 5px 0;"
                "}");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // Название
  setWindowTitle(t.name.split(".")[0]);

  // Заголовок
  QLabel *textLabel = new QLabel(this);
  textLabel->setText(t.name.split(".")[0]);
  QFont font = textLabel->font();

  font.setPointSize(14);
  font.setBold(true);
  textLabel->setFont(font);
  textLabel->setAlignment(Qt::AlignTop | Qt::AlignCenter);

  mainLayout->addWidget(textLabel);

  // Таблица
  table = new QTableWidget(this);

  // Запрет редактирования таблицы
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  // Отключение фокуса на таблице
  table->setFocusPolicy(Qt::NoFocus);
  table->setSelectionMode(QAbstractItemView::NoSelection);
  // Включение сортировки столбцов
  table->setSortingEnabled(true);
  table->horizontalHeader()->setSortIndicatorShown(true);

  // Размеры
  table->horizontalHeader()->setStretchLastSection(true);
  table->verticalHeader()->hide();

  // Шапка таблицы
  table->setRowCount(t.getSize());
  table->setColumnCount(2);
  table->setHorizontalHeaderLabels(QStringList() << "rank"
                                                 << "name");
  // Данные
  int count = 0;
  foreach (QVector<QString> rows, t.data) {
    table->setItem(count, 0, new FloatItem(rows[0]));
    table->setItem(count++, 1, new QTableWidgetItem(rows[1]));
  }

  mainLayout->addWidget(table);
}

// Событие закрытия окна
void TableDialog::closeEvent(QCloseEvent *event) {
  emit shutdown();
  event->accept();
}