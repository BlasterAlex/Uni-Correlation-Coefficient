// Инициализация переменных состояний
chrome.storage.sync.get(['csv'], function (result) {
  if (result.csv === undefined) {
    chrome.storage.sync.set({
      csv: ";"
    });
  }
});

// Изменение значка при запуске скрипта
function checkUrl() {
  chrome.tabs.getSelected(null, function (activeTab) {
    if (activeTab.url.match("https://www.timeshighereducation.com/world-university-rankings/2018*") ||
      activeTab.url.match("https://cwur.org/2018-19.php") ||
      activeTab.url.match("http://roundranking.com/ranking/world-university-rankings.html*") ||
      activeTab.url.match("http://www.shanghairanking.com/ARWU*")) {

      chrome.browserAction.setIcon({
        path: {
          "16": "data/images/icons/active-icon-16.png",
          "32": "data/images/icons/active-icon-32.png"
        }
      });
    } else {
      chrome.browserAction.setIcon({
        path: {
          "16": "data/images/icons/inactive-icon-16.png",
          "32": "data/images/icons/inactive-icon-32.png"
        }
      });
    }
  });
};
checkUrl();

// Изменение иконки расширения при изменении вкладки
chrome.tabs.onActivated.addListener(function () {
  checkUrl();
});

// Изменение иконки расширения при обновлении url
chrome.tabs.onUpdated.addListener(function () {
  checkUrl();
});
