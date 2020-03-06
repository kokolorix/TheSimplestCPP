@echo off
cd /d "%~dp0"
echo %~dp0

set url_file=%~dpn0.url
set index_file=%~dp0bin\html\index.html
set index_file=%index_file:\=/%
echo.>"%url_file%"
echo [InternetShortcut]>>"%url_file%"
echo URL_fileURL_file="file:///%index_file%">>"%url_file%"
REM echo WorkingDirectory=.>>"%url_file%"
echo IconIndex_file=0 >>"%url_file%"
echo IconFile=code_cplusplus.ico>>"%url_file%"

call start "" "%index_file%"