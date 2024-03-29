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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>
#include <QVector>

// Для хранения частей таблиц
struct WebRes {
  QString name;
  int number;

  QString getFullName() { return name + QString::number(number); }
};

// Разбор названий таблиц с номерами
WebRes nameParsing(QString);

// Запись значений
void setSetting(QString, QVariant);
void setSetting(QString, QVector<QVariant>);

// Получение значений
QVariant getSetting(QString);
QVector<QVariant> getVectorSettings(QString);
QVariant getWebRes(QString);

// Проверка файлов настроек
bool settingsFileIsValid();
bool webResFileIsValid();

#endif // SETTINGS_H