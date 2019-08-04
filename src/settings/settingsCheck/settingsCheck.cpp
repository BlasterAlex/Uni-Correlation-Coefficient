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

#include "../settings.hpp"

// Проверка файла настроек
bool settingsFileIsValid() {

  // Проверка на наличие папки
  QString configDir = "config";
  if (!QDir(configDir).exists()) {

    QString outText;
    outText += "Не найдена папка <b>" + configDir + "</b> в директории приложения";
    QMessageBox::critical(0, "Ошибка файла конфигурации", outText);

    return false;
  }

  // Проверка на наличие файла
  QString config = configDir + QDir::separator() + "settings.ini";
  if (!QFile::exists(config)) {

    QString outText;
    outText += "Не найден конфигурационный файл <b>" + config + "</b>";
    QMessageBox::critical(0, "Ошибка файла конфигурации", outText);

    return false;
  }

  // Проверка на наличие настроек
  QVector<QString> settings = {"app/ico",    "output/coeff", "output/dir", "uploads/changesDir", "uploads/csvSeparator", "uploads/extensions",
                               "uploads/dir"};

  foreach (QString setting, settings) {
    QVariant res = getSetting(setting);
    if (res.isNull()) {
      qDebug() << res;

      QString outText;
      outText += "Ошибка конфигурационного файла <b>" + config + "</b>";
      outText += "<br><br>Отсутствует или имеет пустое значение настройка <b>" + setting + "</b>";
      QMessageBox::critical(0, "Ошибка файла конфигурации", outText);

      return false;
    }
  }

  // Все хорошо
  return true;
}