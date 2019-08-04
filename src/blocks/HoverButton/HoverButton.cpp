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
#include <QGraphicsDropShadowEffect>
#include <QHoverEvent>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "HoverButton.hpp"

HoverButton::HoverButton(QWidget *parent) : QPushButton(parent) {}

HoverButton::HoverButton(QPixmap pixmap, QWidget *parent) : QPushButton(parent) {
  this->setIcon(QIcon(pixmap));
  this->setIconSize(pixmap.rect().size());
  this->setFixedSize(pixmap.rect().size() + QSize(7, 7));
}

HoverButton::HoverButton(QString text, QWidget *parent) : QPushButton(parent) { this->setText(text); }

void HoverButton::mousePressEvent(QMouseEvent *) { emit clicked(); }

void HoverButton::hoverLeave(QHoverEvent *) {
  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
  effect->setBlurRadius(0);
  effect->setXOffset(0);
  effect->setYOffset(0);
  this->setGraphicsEffect(effect);
  this->move(this->pos() + QPoint(0, 1));
  repaint();
}

void HoverButton::hoverEnter(QHoverEvent *) {
  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
  effect->setBlurRadius(15);
  effect->setXOffset(2);
  effect->setYOffset(2);
  effect->setColor("#ababab");
  this->setGraphicsEffect(effect);
  this->move(this->pos() + QPoint(0, -1));
  repaint();
}

bool HoverButton::event(QEvent *event) {
  switch (event->type()) {
  case QEvent::HoverLeave:
    hoverLeave(static_cast<QHoverEvent *>(event));
    return true;
    break;
  case QEvent::HoverEnter:
    hoverEnter(static_cast<QHoverEvent *>(event));
    return true;
    break;
  default:
    break;
  }
  return QWidget::event(event);
}