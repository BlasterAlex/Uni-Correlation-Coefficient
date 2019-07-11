$(document).ready(function () {

  // Получение текущего состояния
  chrome.storage.sync.get(['csv'], function (result) {
    $("#csv").val(result.csv);
    $("#csv").addClass('has-content');
  });

  // Событие ввода 
  $("#csv")
    .keyup(function () {
      let val = $(this).val();
      if (val && val.length) { // если есть значение в поле ввода
        $("#csv").addClass('has-content');
        chrome.storage.sync.set({
          csv: val
        });
      } else
        $("#csv").removeClass('has-content');
    })
    .keyup();

  // Переход по ссылке
  $('body').on('click', 'a', function () {
    chrome.tabs.create({ url: $(this).attr('href') });
    return false;
  });
});