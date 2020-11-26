@echo off
cd /d "%~dp0"
@REM echo %~dp0

call:doIt

goto:eof

:doIt
call doxygen Doxyfile
call bin\open-doc
goto:eof
