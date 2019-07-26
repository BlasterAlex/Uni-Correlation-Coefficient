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
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QVector>
#include <algorithm>

#include "settings.hpp"

// Запись значений
void setSetting(QString name, QVariant value) {
  QSettings settings("config/settings.ini", QSettings::IniFormat);
  settings.setValue(name, value);
  settings.sync();
}

void setSetting(QString name, QVector<QVariant> array) {
  QSettings settings("config/settings.ini", QSettings::IniFormat);
  settings.beginWriteArray(name);
  int i = 0;
  foreach (QVariant item, array) {
    settings.setArrayIndex(i++);
    settings.setValue("id", item);
  }
  settings.sync();
  settings.endArray();
}

// Получение значений
QVariant getSetting(QString name) {
  QSettings settings("config/settings.ini", QSettings::IniFormat);
  return settings.value(name);
}

QVariant getWebRes(QString name) {
  QSettings settings("config/web-resources.ini", QSettings::IniFormat);
  return settings.value(name);
}

QVector<QVariant> getVectorSettings(QString name) {
  QSettings settings("config/settings.ini", QSettings::IniFormat);
  QVector<QVariant> array;

  int size = settings.beginReadArray(name);
  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    array.push_back(settings.value("id"));
  }
  settings.endArray();

  return array;
}