#include "settings.hpp"

#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QVector>

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

QVariant getSetting(QString name) {
  QSettings settings("config/settings.ini", QSettings::IniFormat);
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