#include "UploadForm.hpp"

#include <QDebug>
#include <QDragEnterEvent>
#include <QEvent>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QGroupBox>
#include <QHoverEvent>
#include <QLabel>
#include <QMimeData>
#include <QVBoxLayout>
#include <QWidget>

UploadForm::UploadForm(QWidget *parent) : QGroupBox(parent) {

  setAcceptDrops(true);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);

  setFixedWidth(180);
  setFixedHeight(170);

  setStyleSheet("UploadForm {"
                "  border: 2px solid rgba(178, 178, 178, 255);"
                "  border-radius: 10px; "
                "  background-color: #e6f2f5;"
                "}");

  QVBoxLayout *vbox = new QVBoxLayout(this);
  vbox->setAlignment(Qt::AlignTop);

  QImage image("data/images/plus.png");
  QLabel *imageLabel = new QLabel(this);

  imageLabel->setGeometry(100, 100, 100, 100);
  imageLabel->setAlignment(Qt::AlignCenter);
  imageLabel->setPixmap(QPixmap::fromImage(image.scaled(90, 90, Qt::KeepAspectRatio)));

  vbox->addWidget(imageLabel);

  textLabel = new QLabel(this);
  textLabel->setText("Выберите файлы");
  textLabel->setAlignment(Qt::AlignCenter);

  vbox->addWidget(textLabel);

  QLabel *textLabelBottom = new QLabel(this);
  textLabelBottom->setText("или перетащите их сюда");
  textLabelBottom->setAlignment(Qt::AlignCenter);
  vbox->addWidget(textLabelBottom);
}

// Нажатие на форму
void UploadForm::mousePressEvent(QMouseEvent *) {
  files.clear();
  files = QFileDialog::getOpenFileNames(this, "Выберите файлы для загрузки");

  if (!files.empty())
    emit submit();
}

// DragAndDrop
void UploadForm::dragEnterEvent(QDragEnterEvent *e) {
  if (e->mimeData()->hasUrls()) {
    setStyleSheet("UploadForm {"
                  "  border: 2px solid rgba(178, 178, 178, 255);"
                  "  border-radius: 10px; "
                  "  background-color: #fff;"
                  "}");

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(15);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor("#828282");
    this->setGraphicsEffect(effect);
    this->move(this->pos() + QPoint(0, -2));

    e->acceptProposedAction();
  }
}

void UploadForm::dragLeaveEvent(QDragLeaveEvent *e) {
  setStyleSheet("UploadForm {"
                "  border: 2px solid rgba(178, 178, 178, 255);"
                "  border-radius: 10px; "
                "  background-color: #e6f2f5;"
                "}");

  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
  effect->setBlurRadius(0);
  effect->setXOffset(0);
  effect->setYOffset(0);
  this->setGraphicsEffect(effect);
  this->move(this->pos() + QPoint(0, 2));

  e->accept();
}

void UploadForm::dropEvent(QDropEvent *e) {
  files.clear();
  foreach (const QUrl &url, e->mimeData()->urls()) {
    QString fileName = url.toLocalFile();
    files << fileName;
  }

  if (!files.empty())
    emit submit();
}

// Событие наведения мыши
void UploadForm::hoverLeave(QHoverEvent *) {
  setStyleSheet("UploadForm {"
                "  border: 2px solid rgba(178, 178, 178, 255);"
                "  border-radius: 10px; "
                "  background-color: #e6f2f5;"
                "}");

  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
  effect->setBlurRadius(0);
  effect->setXOffset(0);
  effect->setYOffset(0);
  this->setGraphicsEffect(effect);
  this->move(this->pos() + QPoint(0, 2));

  textLabel->setStyleSheet("font-weight: normal; text-decoration: none");
  repaint();
}

void UploadForm::hoverEnter(QHoverEvent *) {
  setStyleSheet("UploadForm {"
                "  border: 2px solid rgba(178, 178, 178, 255);"
                "  border-radius: 10px; "
                "  background-color: #fff;"
                "}");

  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
  effect->setBlurRadius(15);
  effect->setXOffset(5);
  effect->setYOffset(5);
  effect->setColor("#828282");
  this->setGraphicsEffect(effect);
  this->move(this->pos() + QPoint(0, -2));

  textLabel->setStyleSheet("font-weight: bold; text-decoration: underline");
  repaint();
}

bool UploadForm::event(QEvent *event) {
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
