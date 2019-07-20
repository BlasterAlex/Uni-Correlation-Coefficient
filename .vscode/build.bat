@echo off
@chcp 1251>nul
setlocal

REM Запомнить текущий каталог
set directory=%cd%

REM Вызвать необходимое окружение
call "D:\ALEX\uSoft\Qt\5.13.0\msvc2017_64\bin\qtenv2.bat"
call "D:\ALEX\uSoft\Microsoft Visual Studio\VC\Auxiliary\Build\vcvars64.bat"

REM Вернуться в каталог проекта
cd /D %directory%
echo %cd%

REM Компиляция
start /WAIT /B D:\ALEX\uSoft\Qt\5.13.0\msvc2017_64\bin\qmake.exe -config release
start /WAIT /B nmake

REM Переместить исполняемый файл в корень проекта
move release\uni-correlation-coefficient.exe uni-correlation-coefficient.exe