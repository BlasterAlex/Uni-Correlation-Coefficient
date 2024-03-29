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
