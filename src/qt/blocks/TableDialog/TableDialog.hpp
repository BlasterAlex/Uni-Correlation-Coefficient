#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../../Table/Table.hpp"

class TableDialog : public QDialog {
  Q_OBJECT

public:
  explicit TableDialog(int, Table, QWidget *parent = 0);
  virtual ~TableDialog() {}

protected:
  void closeEvent(QCloseEvent *); // закрытие окна

private:
  int num; // номер таблицы в массиве

signals:
  void shutdown();

  friend class Coefficients;
};

#endif // TABLEDIALOG_H
