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