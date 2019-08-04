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

  connect(button, &HoverButton::clicked, this, &FileBlock::remove);

  setFixedHeight(50);
  vbox->setContentsMargins(5, 5, 5, 5);
}