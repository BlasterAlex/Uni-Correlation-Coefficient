#ifndef UPLOADFORM_H
#define UPLOADFORM_H

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QGroupBox>
#include <QHoverEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QString>

class UploadForm : public QGroupBox {
  Q_OBJECT

public:
  explicit UploadForm(QWidget *parent = 0);
  virtual ~UploadForm() {}

private:
  QStringList files;
  QLabel *textLabel;

protected:
  // События мыши
  void mousePressEvent(QMouseEvent *);

  // DragAndDrop
  void dragEnterEvent(QDragEnterEvent *);
  void dragLeaveEvent(QDragLeaveEvent *);
  void dropEvent(QDropEvent *);

  // Событие наведения мыши
  void hoverLeave(QHoverEvent *);
  void hoverEnter(QHoverEvent *);
  bool event(QEvent *);

signals:
  void submit();

  friend class FileUpload;
};

#endif // UPLOADFORM_H
