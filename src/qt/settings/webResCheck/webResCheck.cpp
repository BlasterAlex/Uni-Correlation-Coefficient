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

#include "../settings.hpp"

// Для хранения частей таблиц
struct PartOfTable {
  QString name;
  int number;

  QString getFullName() { return name + QString::number(number); }
};

// Разбор названий с номерами
PartOfTable nameParsing(QString item) {
  int i;
  QString bufNum;
  PartOfTable res;

  // Выделение числа
  for (i = item.size() - 1; i >= 0; --i) {
    if (item[i].isNumber())
      bufNum.insert(0, item[i]);
    else
      break;
  }

  // Запись полученных значений
  if (!bufNum.isEmpty()) {
    res.name = item.mid(0, i + 1);
    res.number = bufNum.toInt();
  } else {
    res.name = item;
    res.number = -1;
  }

  return res;
}

// Проверка наличия всех полей для ресурса
bool checkFieldExist(QString name) {
  QString config = "config" + QString(QDir::separator()) + "web-resources.ini";
  QVariant res = getWebRes(name);

  if (res.isNull()) {
    qDebug() << res;

    QString outText;
    outText += "Ошибка конфигурационного файла <b>" + config + "</b>";
    outText += "<br><br>Отсутствует или имеет пустое значение настройка <b>" + name + "</b>";
    QMessageBox::critical(0, "Ошибка файла ресурсов", outText);

    return false;
  }

  return true;
}

// Файла ресурсов
bool webResFileIsValid() {

  // Проверка на наличие файла
  QString config = "config" + QString(QDir::separator()) + "web-resources.ini";
  if (!QFile::exists(config)) {

    QString outText;
    outText += "Не найден файл ресурсов <b>" + config + "</b>";
    QMessageBox::critical(0, "Ошибка файла ресурсов", outText);

    return false;
  }

  // Проверка наличия списка названий ресурсов
  QString resInfo = "info/resources";
  QVariant resources = getWebRes(resInfo);

  if (!checkFieldExist(resInfo))
    return false;

  // Проверка по типам хранимых данных
  if (strcmp(resources.typeName(), "QString") == 0) { // задано всего одно название

    // Проверка названия ресурса
    QString res = resources.toString();
    if (res[res.size() - 1].isNumber()) { // оканчивается на цифру

      QString outText;
      outText += "Ошибка файла ресурсов <b>" + config + "</b>";
      outText += "<br><br>Необходимо ввести остальные части таблицы <b>" + res.mid(0, res.size() - 1) + "</b>";
      QMessageBox::critical(0, "Ошибка файла ресурсов", outText);

      return false;
    }

    // Проверка полей
    QVector<QString> fields = {res + "/href", res + "/headline", res + "/rank", res + "/name"};

    foreach (QString field, fields)
      if (!checkFieldExist(field))
        return false;

  } else if (strcmp(resources.typeName(), "QStringList") == 0) { // задан список названий

    // Список названий ресурсов
    QStringList resList = resources.toStringList();

    // Список частей таблиц, формируемый в процессе прохода по списку
    QVector<QVector<PartOfTable>> parts;

    // Проход по всем ресурсам в списке
    foreach (QString res, resList) {

      // Проверка на дублирование
      int countOf = std::count(resList.begin(), resList.end(), res);
      if (countOf != 1) {

        // Для корректного вывода
        QString times;
        if (countOf > 4)
          times = "раз";
        else
          times = "раза";

        QString outText;
        outText += "Ошибка файла ресурсов <b>" + config + "</b>";
        outText += "<br><br>Таблица <b>" + res + "</b> повторяется " + QString::number(countOf) + " " + times;
        outText += "<br><br>Таблицы, разделенные на несколько частей, должны нумероваться";
        outText += "<pre>    Name1, Name2, Name3, ...</pre>";
        QMessageBox::critical(0, "Ошибка файла ресурсов", outText);

        return false;
      }

      // Отбор таблиц, разделенных на части
      PartOfTable currentPart = nameParsing(res);

      if (currentPart.number != -1) { // это часть таблицы

        // Поиск названия в списке частей
        auto found =
            std::find_if(parts.begin(), parts.end(), [&cn = currentPart.name](const QVector<PartOfTable> &part) { return part[0].name == cn; });

        if (found != parts.end())
          found->push_back(currentPart); // добавить в существующий список частей
        else
          parts.push_back(QVector<PartOfTable>({currentPart})); // часть новой таблицы

      } else { // это обычная таблица

        // Проверка всех полей для текущего ресурса
        QVector<QString> fields = {res + "/href", res + "/headline", res + "/rank", res + "/name"};

        foreach (QString field, fields)
          if (!checkFieldExist(field))
            return false;
      }
    }

    // Проверка частей таблиц
    foreach (QVector<PartOfTable> resParts, parts) {

      // Сортировка частей по значению number
      std::sort(resParts.begin(), resParts.end(), [](const PartOfTable &a, const PartOfTable &b) { return a.number < b.number; });

      // Нумерация частей
      if (resParts[0].number != 1) { // нумерация начинается не с 1
        QString outText;
        outText += "Ошибка файла ресурсов <b>" + config + "</b>";
        outText += "<br><br>Нумерация частей таблиц должна начинаться с 1";
        outText += "<br><br>Для таблиц <b>" + resParts[0].name + "</b> нумерация начинается с <b>" + QString::number(resParts[0].number) + "</b>";
        QMessageBox::critical(0, "Ошибка файла ресурсов", outText);

        return false;
      } else {
        for (int i = 1; i < resParts.size(); ++i)
          if (resParts[i].number != i + 1) { // неправильная нумерация
            QString outText;
            outText += "Ошибка файла ресурсов <b>" + config + "</b>";
            outText += "<br><br>Для таблиц <b>" + resParts[0].name + "</b> имеется разрыв нумерации <b>с " + QString::number(resParts[i - 1].number) +
                       " по " + QString::number(resParts[i].number) + "</b>";
            QMessageBox::critical(0, "Ошибка файла ресурсов", outText);

            return false;
          }
      }

      // Проверка всех полей
      foreach (PartOfTable resPart, resParts) {
        QVector<QString> fields = {resPart.getFullName() + "/href", resPart.getFullName() + "/headline", resPart.getFullName() + "/rank",
                                   resPart.getFullName() + "/name"};

        foreach (QString field, fields)
          if (!checkFieldExist(field))
            return false;
      }
    }
  } else { // задан неподдерживаемый тип данных

    QString outText;
    outText += "Ошибка файла ресурсов <b>" + config + "</b>";
    outText += "<br><br>Недопустимый тип данных в <b>" + resInfo + "</b>";
    QMessageBox::critical(0, "Ошибка файла ресурсов", outText);

    return false;
  }

  // Все хорошо
  return true;
}