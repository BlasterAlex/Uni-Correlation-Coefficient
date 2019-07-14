#include "Table.hpp"
#include "../settings/settings.hpp"

#include <QCollator>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>

#define Iterator QMutableVectorIterator<QVector<QString>>

// Чтение таблицы из файла
Table::Table(QString filename) : name(filename) {

  QString dir = getSetting("uploads/dir").toString() + QDir::separator();
  QFile file(dir + name);
  QString separator = getSetting("uploads/csvSeparator").toString();

  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    qDebug() << "Файл " + name + " недоступен!";
  } else {
    // Поток для извлечения данных из файла
    QTextStream in(&file);

    // Одна таблица
    QVector<QVector<QString>> table;

    // Чтение файла до конца
    int index = 0;
    while (!in.atEnd()) {
      QString line = in.readLine();
      QVector<QString> tableLine;

      for (QString item : line.split(separator)) {
        tableLine.push_back(item);
      }
      tableLine.push_back(QString::number(++index));

      table.push_back(tableLine);
    }
    file.close();

    // Сохранение новой таблицы
    data = table;

    // Сохранение максимального значения ранга
    maxRank = data[data.size() - 1][0].toFloat();
  }
}

// Поиск в таблице
int Table::search(QString keyName) {
  int index = 0;
  foreach (QVector<QString> tableLine, data) {
    if (tableLine[1].compare(keyName) == 0)
      return index;
    ++index;
  }
  return -1;
}

// Запись таблицы в файл
void Table::writeToCSV() {
  QString path = getSetting("uploads/changesDir").toString() + QDir::separator();
  QString separator = getSetting("uploads/csvSeparator").toString();

  QFile file(path + name);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream stream(&file);
    foreach (QVector<QString> tableLine, data)
      stream << tableLine[0] << separator << tableLine[1] << separator << tableLine[2] << endl;
    file.close();
  }
}

// Сравнение с другой таблицей
void Table::analysisWith(Table &another) {

  float change = 0;       // величина сдвига в процессе удаления записей
  float prevRank = -1;    // предыдущий ранг
  float prevRemRank = -1; // предыдущий удаленный ранг
  float lastModified = 0; // величина последнего изменения

  /*** Алгоритм:
   * производится проход по таблице с поиском текущего названия в другой таблице
   * при нахождении несовпадения, запись из первой таблицы удаляется
   * если нет других записей с таким же рангом, removed увеличивается на 1
   */

  Iterator iter(this->data);
  while (iter.hasNext()) {

    int res = another.search(iter.next()[1]);
    float rank = iter.value()[0].toFloat();

    if (res != -1) { // совпадение - не думаю

      // если последняя запись == последней удаленной == текущей
      if (prevRemRank == rank && prevRank == rank) { // удаленный ранг не единственный (сдвига нет)
        prevRemRank = -1;                            // забыть удаленную запись, она не учитывается
        change -= lastModified;                      // восстановить значение сдвига
      }

      prevRank = rank; // запомнить текущий ранг

      // Вычесть из текущего ранга текущее значение сдвига
      iter.value()[0] = QString::number(rank - change);

    } else { // различие

      iter.remove(); // удалить запись

      if (prevRank != rank) {           // если ранг текущей записи отличен от предыдущей
        prevRemRank = rank;             // запомнить удаленный ранг
        lastModified = rank - prevRank; // разница между рангами
        change += lastModified;         // увеличить величину сдвига
      }

      prevRank = rank; // запомнить текущий ранг
    }
  }

  // Обновление индексов
  for (int i = 0; i < data.size(); ++i)
    data[i][2] = QString::number(i + 1);

  // Обновление максимального значения ранга
  maxRank = data[data.size() - 1][0].toFloat();
}

// Сортировка таблицы по названиям
void Table::sortByName() {
  QCollator collator;
  collator.setNumericMode(true);

  std::sort(data.begin(), data.end(), [&collator](const QVector<QString> &a, const QVector<QString> &b) { return collator.compare(a[1], b[1]) < 0; });
}

// Получение размера таблицы
int Table::getSize() { return data.size(); }