#ifndef FLOATITEM_H
#define FLOATITEM_H

#include <QString>
#include <QTableWidgetItem>

class FloatItem : public QTableWidgetItem {

public:
  explicit FloatItem(const QString text) : QTableWidgetItem(text) { setTextAlignment(Qt::AlignCenter); }
  virtual ~FloatItem() {}

  bool operator<(const QTableWidgetItem &other) const { return (this->text().toFloat() < other.text().toFloat()); }
};

#endif // FLOATITEM_H