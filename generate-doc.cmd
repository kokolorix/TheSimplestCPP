@echo off
cd /d "%~dp0"
echo %~dp0

call:doIt

goto:eof

:doIt
call doxygen Doxyfile
call bin\open-doc
goto:eof
