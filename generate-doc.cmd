@echo off
cd /d "%~dp0"
@REM echo %~dp0
set path=%cd%\tools;%cd%\tools\doxygen;%cd%\tools\graphviz;%path%

call:doIt

goto:eof

:doIt
call doxygen Doxyfile
call nircmd shortcut "%cd%\pages\docs\index.html" "%cd%" "show-doc" "" "%cd%\bin\window_earth.ico"
goto:eof
