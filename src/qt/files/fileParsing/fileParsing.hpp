#ifndef FILEPARSING_H
#define FILEPARSING_H

#include <QString>
#include <QVector>

#include "../../Table/Table.hpp"

QVector<Table> getParsedTables(QVector<QString> files);

#endif // FILEPARSING_H