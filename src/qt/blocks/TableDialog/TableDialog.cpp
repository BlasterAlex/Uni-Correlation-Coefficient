#include <QCloseEvent>
#include <QDialog>
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "TableDialog.hpp"

TableDialog::TableDialog(Table t, QWidget *parent) : QDialog(parent) {

  setMinimumWidth(400);
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
                ""
                "QHeaderView::section:vertical {"
                "  border-left: 1px solid #fffff8;"
                "}"
                "QLabel {"
                "  margin: 5px 0;"
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
  font.setPointSize(15);
  font.setBold(true);
  textLabel->setFont(font);

  mainLayout->addWidget(textLabel);

  // Таблица
  QTableWidget *table = new QTableWidget(this);

  // Запрет редактирования таблицы
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Отключение фокуса на таблице
  table->setFocusPolicy(Qt::NoFocus);
  table->setSelectionMode(QAbstractItemView::NoSelection);

  // Размеры
  table->horizontalHeader()->setStretchLastSection(true);
  table->verticalHeader()->hide();

  table->setRowCount(t.getSize());
  table->setColumnCount(2);
  table->setHorizontalHeaderLabels(QStringList() << "rank"
                                                 << "name");
  int count = 0;
  foreach (QVector<QString> rows, t.data) {
    QTableWidgetItem *first = new QTableWidgetItem(rows[0]);
    first->setTextAlignment(Qt::AlignCenter);
    table->setItem(count, 0, first);
    table->setItem(count++, 1, new QTableWidgetItem(rows[1]));
  }

  mainLayout->addWidget(table);
}

// Событие закрытия окна
void TableDialog::closeEvent(QCloseEvent *event) {
  emit shutdown();
  event->accept();
}