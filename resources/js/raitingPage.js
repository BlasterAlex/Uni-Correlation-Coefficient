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

// Среднее значение диапазона последовательности элементов
function rangeAverage(first, last) {
  let sum = 0;
  let count = 0;

  ++first;
  ++last;

  while (first < last) {
    sum += first;
    ++first;
    ++count;
  }

  return sum / count;
}

// Разбор таблицы
function parseTable(table) {
  let universities = new Array;

  var prevRank = -1; // предыдущий ранг
  var beginRange = -1; // индекс начала диапазона одинаковых рангов

  $(table.headline).each(function (i, el) {
    let buffer = new Array;

    // Получение ранга
    let rank = $(el).find(table.rank).text();
    let ranks = rank.split('–');
    if (ranks.length < 2) ranks = rank.split('-');

    if (ranks.length == 2) { // два числа (задан диапазон)

      rank = (parseInt(ranks[0]) + parseInt(ranks[1])) / 2; // среднее значение диапазона

      // Если есть диапазон одинаковых рангов
      if (beginRange != i - 1 && beginRange != -1) {

        // Среднее арифметическое диапазона
        average = rangeAverage(beginRange, i);

        // Обновить значения из данного диапазона
        for (let index = beginRange; index < universities.length; ++index)
          universities[index][0] = average;
      }

      // Сбросить значения (диапазон не учитывается)
      prevRank = -1;
      beginRange = -1;

    } else {
      rank = rank.split('+')[0]; // убрать '+' в конце числа

      if (rank.slice(0, 1) === '=') // '=' перед числом
        rank = rank.slice(1);
      rank = parseFloat(rank);

      // Eсли текущее значение не равно предыдущему (не входит в диапазон одинаковых рангов)
      if (prevRank != parseFloat(rank)) {
        // Если есть диапазон одинаковых рангов
        if (beginRange != i - 1 && beginRange != -1) {

          // Среднее арифметическое диапазона
          average = rangeAverage(beginRange, i);

          // Обновить значения диапазона
          for (let index = beginRange; index < universities.length; ++index)
            universities[index][0] = average;

        }
        beginRange = i; // запомнить текущую позицию 
      }

      prevRank = parseFloat(rank); // запомнить текущий ранг

      // Если сбился порядок рангов (предыдущий ранг больше текущего)
      if (universities.length)
        if (parseFloat(universities[i - 1][0]) > rank)
          rank = i + 1;
    }

    // Запись в массив
    buffer.push(rank);
    buffer.push($(el).find(table.name).text());
    universities.push(buffer);
  });

  // Если остался диапазон после прохода
  if (beginRange != universities.length - 1 && beginRange != -1) {
    // Среднее арифметическое диапазона
    average = rangeAverage(beginRange, universities.length);

    // Обновить значения диапазона
    for (let index = beginRange; index < universities.length; ++index)
      universities[index][0] = average;
  }

  return universities;
}

// Получение таблицы рейтингов
function getTable(param) {
  // Канал для передачи данных в qt
  new QWebChannel(qt.webChannelTransport, function (channel) {
    var api = channel.objects.api;
    window.foo = api;

    // Разбор структуры
    let t = param.split(', ');

    // Получение таблицы со страницы
    let table = parseTable({
      headline: t[0],
      rank: t[1],
      name: t[2]
    });

    // Передача данных в qt
    table.forEach(element => {
      api.addTableRow(element);
    });

    api.endOfTable();
  });
}

// Получение второй части таблицы
function getSecondPart(param) {
  // Канал для передачи данных в qt
  new QWebChannel(qt.webChannelTransport, function (channel) {
    var api = channel.objects.api;
    window.foo = api;

    // Разбор структуры
    let t = param.split(', ');
    let table = {
      headline: t[0],
      rank: t[1],
      name: t[2]
    };

    // Чтение таблицы со страницы
    $(table.headline).each(function (i, el) {
      if (i) {
        let buffer = new Array;

        // Получение ранга
        let rank = $(el).find(table.rank).text();
        let ranks = rank.split('-');

        if (ranks.length == 2) { // два числа
          rank = (parseInt(ranks[0]) + parseInt(ranks[1])) / 2;
          rank = parseInt(rank);
        }

        buffer.push(rank);
        buffer.push($(el).find(table.name).text());

        api.addTableRow(buffer);
      }
    });

    api.endOfTable();
  });
}