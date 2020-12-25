@echo off
cd /d "%~dp0"
@REM echo %~dp0
set path=%cd%\tools\doxygen;%cd%\tools\graphviz;%path%

call:doIt

goto:eof

:doIt
call doxygen Doxyfile
goto:eof
