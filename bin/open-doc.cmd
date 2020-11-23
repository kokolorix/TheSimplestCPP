@echo off
cd /d "%~dp0"
echo %~dp0
pushd .. 
set url_file=%cd%\%~n0.url
popd
echo.>"%url_file%"
set index_file=%~dp0html\index.html
set index_file=%index_file:\=/%
echo [InternetShortcut]>>"%url_file%"
echo URL=file:///%index_file%>>"%url_file%"
echo IDList=>>"%url_file%"
REM echo WorkingDirectory=.>>"%url_file%"
set icon_file=%~dp0code_cplusplus.ico
set icon_file=%icon_file:\=/%
echo IconIndex_file=0 >>"%url_file%"
echo IconFile=%icon_file%>>"%url_file%"

@REM call start "" "%index_file%"