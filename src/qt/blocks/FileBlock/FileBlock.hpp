#ifndef FILEBlOCK_H
#define FILEBlOCK_H

#include <QGroupBox>
#include <QLabel>
#include <QString>
#include <QWidget>

class FileBlock : public QGroupBox {
  Q_OBJECT

public:
  explicit FileBlock(QString text, QWidget *parent = 0);
  virtual ~FileBlock() {}

private:
  QString text;

signals:
  void remove();

  friend class FileUpload;
};

#endif // FILEBlOCK_H
