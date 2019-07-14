#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QLayout>
#include <QMultiMap>
#include <QObject>
#include <QProgressBar>
#include <QString>
#include <QTextStream>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "../Table/Table.hpp"
#include "../settings/settings.hpp"
#include "Coefficients.hpp"

Coefficients::Coefficients(QVector<Table> t, QWidget *parent) : QDialog(parent) {
  tables = t;
  tablesNum = tables.size();

  // Заголовок
  setWindowTitle("Результаты");

  // Основной слой
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // Меню
  setMenu();
  mainLayout->addWidget(toolBar, 0, Qt::AlignTop);

  // Создание поля для вывода результатов
  createContentBlock();
  mainLayout->addWidget(contentBlock, 1, Qt::AlignTop);

  // Полоска загрузки
  createLoader();
  mainLayout->addWidget(progress);

  // Вычисление коэффициентов
  calculation();
}

// Вычисление коэффициентов
void Coefficients::calculation() {

  // Вычисление коэффициента Спирмена
  progress_label->setText("Вычисление коэффициентов корреляции Спирмена...");

  for (int i = 0; i < tablesNum - 1; ++i)
    for (int j = i + 1; j < tablesNum; ++j) {

      float coeff = getSpearmanCoeff(i, j);
      Spearman.push_back(Coeff(i, j, QString::number(coeff)));

      progress_bar->setValue(progress_bar->value() + 1);
    }
  writeToCSV(Spearman, "Spearman");

  // Вычисление коэффициентов Кендалла
  progress_label->setText("Вычисление коэффициентов корреляции Кендалла...");

  for (int i = 0; i < tablesNum - 1; ++i)
    for (int j = i + 1; j < tablesNum; ++j) {

      float coeff = getKendallCoeff(i, j);
      Kendall.push_back(Coeff(i, j, QString::number(coeff)));

      progress_bar->setValue(progress_bar->value() + 1);
    }
  writeToCSV(Kendall, "Kendall");

  // Вычисление коэффициентов методом обмена
  progress_label->setText("Вычисление коэффициентов корреляции методом обмена...");

  for (int i = 0; i < tablesNum - 1; ++i)
    for (int j = i + 1; j < tablesNum; ++j) {

      float coeff = getExchange(i, j);
      Exchange.push_back(Coeff(i, j, QString::number(coeff)));

      progress_bar->setValue(progress_bar->value() + 1);
    }
  writeToCSV(Exchange, "Exchange");

  displayResults();
}

// Вывод результатов в файл
void Coefficients::writeToCSV(QVector<Coeff> coeffs, QString name) {

  // Создание папок для вывода, если нужно
  QString path = getSetting("output/dir").toString();
  QDir dir(path);
  if (!dir.exists()) {
    QDir().mkdir(path);
    qDebug() << "Создана папка " + path;
  }
  path = getSetting("output/coeff").toString();
  dir.setPath(path);
  if (!dir.exists()) {
    QDir().mkdir(path);
    qDebug() << "Создана папка " + path;
  }
  path += QDir::separator();
  QString separator = getSetting("uploads/csvSeparator").toString();

  // Запись в файл
  QFile file(path + name + ".csv");
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream stream(&file);

    stream << "x" << separator << "y" << separator << tr("Коэф.") << endl;

    foreach (Coeff coeff, coeffs)
      stream << coeff.parents[0] << separator << coeff.parents[1] << separator << coeff.val << endl;

    file.close();
  }
}

// Событие закрытия окна
void Coefficients::closeEvent(QCloseEvent *event) {
  emit shutdown();
  event->accept();
}

// Подсчет количества итераций алгоритмов
int Coefficients::iterationsCol(int N) {
  if (N == 0)
    return 0;
  else
    return N + iterationsCol(N - 1);
}
