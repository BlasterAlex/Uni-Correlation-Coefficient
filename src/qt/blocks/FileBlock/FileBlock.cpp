#include "FileBlock.hpp"
#include "../HoverButton/HoverButton.hpp"

#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QString>
#include <QWidget>

FileBlock::FileBlock(QString _text, QWidget *parent) : QGroupBox(parent) {
  QHBoxLayout *vbox = new QHBoxLayout(this);

  text = _text;
  QLabel *textLabel = new QLabel(this);
  textLabel->setText(text);
  vbox->addWidget(textLabel);

  vbox->addItem(new QSpacerItem(20, 0));

  QPixmap pixmap("data/images/red-cross.png");
  pixmap = pixmap.scaled(15, 15, Qt::KeepAspectRatio);
  HoverButton *button = new HoverButton(pixmap, this);
  vbox->addWidget(button);

  connect(button, SIGNAL(clicked()), this, SIGNAL(remove()));

  setFixedHeight(50);
  vbox->setContentsMargins(0, 0, 0, 0);
}