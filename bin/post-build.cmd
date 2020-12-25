@echo off
cd /d "%~dp0"
echo %~n0 %*

set solution=%~1
set project=%~2
set target=%~3

set solutionDir=%~dp1
@REM echo %solutionDir%
pushd %solutionDir%
set path=%solutionDir%bin;%path%

call generate-doc

call:generate-url "%solutionDir%open-doc.url" "%solutionDir%bin\html\index.html" "%solutionDir%bin\window_information.ico" 
call:generate-url "%solutionDir%start-app.url" "%target%" "%solutionDir%bin\window_gear.ico" 
@REM call copy /y  "%target%" "%solutionDir%bin"

popd

goto:eof

:generate-url <shortcut> <url> <icon>
echo %*
set url_file=%~1
set url=%~2
set icon=%3

set url=%url:\=/%

echo.>"%url_file%"
echo [InternetShortcut]>>"%url_file%"
echo URL=file:///%url%>>"%url_file%"
echo IDList=>>"%url_file%"
echo IconIndex=0 >>"%url_file%"
echo IconFile=%icon%>>"%url_file%"

goto:eof
