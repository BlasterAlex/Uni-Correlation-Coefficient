// Получение данных из chrome.storage
function getData(callback) {
  let data = new Object();
  chrome.storage.sync.get(['csv'], function (result) {
    data.separator = result.csv;
    callback(data);
  });
}

// Загрузка таблицы в виде csv файла
function downloadCSV(array, name) {
  getData(function (data) {

    let csvContent = "data:text/csv;charset=utf-8,"
      + array.map(e => e.join(data.separator)).join("\n");

    let encodedUri = encodeURI(csvContent);
    let link = document.createElement('a');
    link.setAttribute('href', encodedUri);
    link.setAttribute('download', name + '.csv');
    document.body.appendChild(link);

    link.click();

  });
}

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
function parseTable(table, name) {
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

    } else if (rank.slice(-1) === '+') { // задан диапазон до конца таблицы
      rank = i + 1;
    } else {

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

  // Загрузка массива в виде csv файла
  downloadCSV(universities, name);
}

// Главная функция
(function (window, undefined) {
  // Нормализация window
  var w;
  if (typeof unsafeWindow != undefined) {
    w = unsafeWindow
  } else {
    w = window;
  }

  // Не запускать скрипт во фреймах
  // без этого условия скрипт будет запускаться несколько раз на странице с фреймами
  if (w.self != w.top) {
    return;
  }

  // Запустить только после загрузки всей страницы
  $(window).bind("load", function () {
    let currUrl = window.location.toString();
    // The World University Rankings
    if (currUrl.match("https://www.timeshighereducation.com/world-university-rankings/*")) {
      if (confirm("Загрузить таблицу рейтингов от THE?")) { // пользователь нажал 'Ok'
        let table = {
          headline: '.table tbody tr',
          rank: '.rank',
          name: '.name .ranking-institution-title'
        }
        parseTable(table, 'World University Rankings (THE)');
      }
    } // CWUR World University Rankings 
    else if (currUrl.match("https://cwur.org/2018-19.php")) {
      if (confirm("Загрузить таблицу рейтингов от CWUR?")) { // пользователь нажал 'Ok'
        let table = {
          headline: '.table tbody tr',
          rank: 'td:nth-child(1)',
          name: 'td:nth-child(2) a'
        }
        parseTable(table, 'World University Rankings (CWUR)');
      }
    } // RUR Round University Rankings 
    else if (currUrl.match("http://roundranking.com/ranking/world-university-rankings.html*")) {
      if (confirm("Загрузить таблицу рейтингов от RUR?")) { // пользователь нажал 'Ok'
        let table = {
          headline: '.big-table tbody tr',
          rank: '.td1',
          name: '.td2 a'
        }
        parseTable(table, 'World University Rankings (RUR)');
      }
    } // Academic Ranking of World Universities
    else if (currUrl.match("http://www.shanghairanking.com/ARWU2018.html")) {
      if (confirm("Загрузить таблицу рейтингов от ARWU?")) { // пользователь нажал 'Ok'
        let table = {
          headline: '#UniversityRanking tbody tr:not(:first)',
          rank: 'td:nth-child(1)',
          name: 'td:nth-child(2) a'
        }
        parseTable(table, 'World University Rankings (ARWU)');
      }
    } // Academic Ranking of World Universities (second part)
    else if (currUrl.match("http://www.shanghairanking.com/ARWU2018Candidates.html")) {
      if (confirm("Загрузить таблицу рейтингов от ARWU?")) { // пользователь нажал 'Ok'
        let universities = new Array;
        $('#UniversityRanking tbody tr').each(function (i, el) {
          if (i) {
            let buffer = new Array;

            // Получение ранга
            let rank = $(el).find('td:nth-child(1)').text();
            let ranks = rank.split('-');

            if (ranks.length == 2) { // два числа
              rank = (parseInt(ranks[0]) + parseInt(ranks[1])) / 2;
              rank = parseInt(rank);
            }

            buffer.push(rank);
            buffer.push($(el).find('td:nth-child(2)').text());

            universities.push(buffer);
          }
        });
        downloadCSV(universities, 'World University Rankings (ARWU)');
      }
    }
  });

})(window);