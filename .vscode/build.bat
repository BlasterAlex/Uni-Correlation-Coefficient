@echo off
@chcp 1251>nul
setlocal

echo %cd%
start /WAIT /B D:\ALEX\uSoft\Qt\5.13.0\mingw73_32\bin\qmake.exe -config release
start /WAIT /B D:\ALEX\uSoft\Qt\Tools\mingw730_32\bin\mingw32-make -j4
move release\uni-correlation-coefficient.exe uni-correlation-coefficient.exe