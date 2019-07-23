#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QString>
#include <QWidget>

#include "../HoverButton/HoverButton.hpp"
#include "FileBlock.hpp"

FileBlock::FileBlock(QString _text, QWidget *parent) : QGroupBox(parent) {

  QHBoxLayout *vbox = new QHBoxLayout(this);

  text = _text;
  QLabel *textLabel = new QLabel(this);
  textLabel->setText(text);
  vbox->addWidget(textLabel, 0, Qt::AlignVCenter);
  vbox->addStretch();

  QPixmap pixmap("data/images/red-cross.png");
  pixmap = pixmap.scaled(15, 15, Qt::KeepAspectRatio);
  HoverButton *button = new HoverButton(pixmap, this);
  vbox->addWidget(button, 0, Qt::AlignVCenter);

  connect(button, SIGNAL(clicked()), this, SIGNAL(remove()));

  setFixedHeight(50);
  vbox->setContentsMargins(5, 5, 5, 5);
}