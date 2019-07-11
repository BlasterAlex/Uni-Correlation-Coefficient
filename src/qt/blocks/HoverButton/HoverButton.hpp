#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QHoverEvent>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QString>
#include <QWidget>

class HoverButton : public QPushButton {
  Q_OBJECT

public:
  explicit HoverButton(QWidget *parent = 0);
  explicit HoverButton(QPixmap, QWidget *parent = 0);
  explicit HoverButton(QString, QWidget *parent = 0);
  virtual ~HoverButton() {}

protected:
  // Нажатие на кнопку
  void mousePressEvent(QMouseEvent *);
  // Событие наведения мыши
  void hoverLeave(QHoverEvent *);
  void hoverEnter(QHoverEvent *);
  bool event(QEvent *);

signals:
  void clicked();
};

#endif // HOVERBUTTON_H
