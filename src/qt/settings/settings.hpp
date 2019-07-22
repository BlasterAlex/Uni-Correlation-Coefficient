#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>
#include <QVector>

void setSetting(QString, QVariant);
void setSetting(QString, QVector<QVariant>);

QVariant getSetting(QString);
QVector<QVariant> getVectorSettings(QString);

QVariant getWebRes(QString);

#endif // SETTINGS_H