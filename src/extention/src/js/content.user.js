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

  // Создание кнопки загрузки
  $('body').prepend('<div class="pulse" style="opacity: 0">' +
    '  <img src="' + chrome.runtime.getURL('data/images/icons/download-icon.png') + '"/>' +
    '</div>');
  $('<style type="text/css">' +
    '.pulse {' +
    '  display: block;' +
    '  border-radius: 50%;' +
    '  background: #00c9ca;' +
    '  cursor: pointer;' +
    '  position: fixed;' +
    '  top: 20px;' +
    '  transition: all 0.2s;' +
    '  right: 20px;' +
    '  border: 1px solid #00c9ca;' +
    '  z-index: 9999;' +
    '}' +
    '' +
    '.pulse img {' +
    '  border-radius: 50%;' +
    '  width: 60px;' +
    '  height: 60px;' +
    '}' +
    '' +
    '.pulse:hover {' +
    '  opacity: 0.7 !important;' +
    '}' +
    '' +
    '.pulse:after {' +
    '  content:"";' +
    '  width: 64px !important;' +
    '  height: 64px !important;' +
    '  border-radius: 50%;' +
    '  border: 4px solid #00c9ca;' +
    '  position: fixed;' +
    '  top:17px;' +
    '  right:17px;' +
    '  animation: pulse 2s ease 0s infinite;' +
    '  box-sizing: border-box;' +
    '}' +
    '' +
    '@keyframes pulse {' +
    '  0% {' +
    '    opacity: 1;' +
    '    transform: scale(1);' +
    '  }' +
    '  80% {' +
    '    opacity: 0;' +
    '    transform: scale(2);' +
    ' }' +
    '  100% {' +
    '    opacity: 0;' +
    '    transform: scale(2.5);' +
    '  }' +
    '}' +
    '</style>')
    .appendTo('head');

  // Запустить только после загрузки всей страницы
  $(window).bind("load", function () {
    let currUrl = window.location.toString();
    // The World University Rankings
    if (currUrl.match("https://www.timeshighereducation.com/world-university-rankings/*")) {

      // Событие нажатия кнопки загрузки
      $(document).on('click', '.pulse', function () {
        let table = {
          headline: '.table tbody tr',
          rank: '.rank',
          name: '.name .ranking-institution-title'
        }
        // Загрузка массива в виде csv файла
        downloadCSV(parseTable(table), 'THE');
      });

      if (confirm("Загрузить таблицу рейтингов от THE сейчас?")) { // пользователь нажал 'Ok'
        $('.pulse').trigger('click');
      } else { // иначе - показать кнопку
        $('.pulse').animate({
          opacity: 1,
        }, 300);
      }
    } // CWUR World University Rankings 
    else if (currUrl.match("https://cwur.org/2018-19.php")) {

      // Событие нажатия кнопки загрузки
      $(document).on('click', '.pulse', function () {
        let table = {
          headline: '.table tbody tr',
          rank: 'td:nth-child(1)',
          name: 'td:nth-child(2) a'
        }
        // Загрузка массива в виде csv файла
        downloadCSV(parseTable(table), 'CWUR');
      });

      if (confirm("Загрузить таблицу рейтингов от CWUR сейчас?")) { // пользователь нажал 'Ok'
        $('.pulse').trigger('click');
      } else { // иначе - показать кнопку
        $('.pulse').animate({
          opacity: 1,
        }, 300);
      }
    } // RUR Round University Rankings 
    else if (currUrl.match("http://roundranking.com/ranking/world-university-rankings.html*")) {

      // Событие нажатия кнопки загрузки
      $(document).on('click', '.pulse', function () {
        let table = {
          headline: '.big-table tbody tr',
          rank: '.td1',
          name: '.td2 a'
        }
        // Загрузка массива в виде csv файла
        downloadCSV(parseTable(table), 'RUR');
      });

      if (confirm("Загрузить таблицу рейтингов от RUR сейчас?")) { // пользователь нажал 'Ok'
        $('.pulse').trigger('click');
      } else { // иначе - показать кнопку
        $('.pulse').animate({
          opacity: 1,
        }, 300);
      }
    } // Academic Ranking of World Universities
    else if (currUrl.match("http://www.shanghairanking.com/ARWU2018.html")) {

      // Событие нажатия кнопки загрузки
      $(document).on('click', '.pulse', function () {
        let table = {
          headline: '#UniversityRanking tbody tr:not(:first)',
          rank: 'td:nth-child(1)',
          name: 'td:nth-child(2) a'
        }
        // Разбор таблицы
        let universities = parseTable(table);
        // Сохранение таблицы
        chrome.storage.local.set({
          keptTable: universities
        }, function () { // переход на страницу с продолжением таблицы
          document.location.href = "http://www.shanghairanking.com/ARWU2018Candidates.html";
        });
      });

      if (confirm("Загрузить таблицу рейтингов от ARWU сейчас?")) { // пользователь нажал 'Ok'
        $('.pulse').trigger('click');
      } else { // иначе - показать кнопку
        $('.pulse').animate({
          opacity: 1,
        }, 300);
      }
    } // Academic Ranking of World Universities (second part)
    else if (currUrl.match("http://www.shanghairanking.com/ARWU2018Candidates.html")) {

      // Событие нажатия кнопки загрузки
      $(document).on('click', '.pulse', function () {
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
        // Проверка на наличие сохраненной таблицы
        chrome.storage.local.get(['keptTable'], function (result) {
          if (result.keptTable === undefined) { // загрузить текущую таблицу
            downloadCSV(universities, 'ARWU');
          } else { // объединить текущую таблицу с предыдущей
            universities = result.keptTable.concat(universities);
            // Загрузка таблицы
            downloadCSV(universities, 'ARWU');
            // Освобождение памяти
            chrome.storage.local.remove(['keptTable'], function () { });
          }
        });
      });

      if (confirm("Загрузить таблицу рейтингов от ARWU сейчас?")) { // пользователь нажал 'Ok'
        $('.pulse').trigger('click');
      } else { // иначе - показать кнопку
        $('.pulse').animate({
          opacity: 1,
        }, 300);
      }
    }
  });

})(window);