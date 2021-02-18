@echo off
cd  /d "%~dp0"
title %~n0 - %cd%

set path=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\Tools;%path%
call VsDevCmd
call MSBuild TheSimplestWinApp.sln -t:Rebuild -p:Configuration=Release -p:Platform=x64
set path=%cd%\tools;%path%
call nircmd shortcut "%cd%\x64\Release\TheSimplestWinApp.exe" "%cd%" "start-app" "" "%cd%\bin\window_gear.ico"
goto:wait


echo warte max 10s
for /l %%i in (1, 1, 10) do (
	call ping -n 2 localhost>nul
    echo 9
    call:title %%i
)
goto:eof

:wait
set wait=10
choice /c wq /t %wait% /d q /m "W=pause, Q=quit(%wait%s)"
if errorlevel 2 goto:eof
if errorlevel 1 goto:exit

:exit
pause
goto:eof

:title
set /a s= 10 - %~1
title %~n0 - %s% s
@REM echo -ne '\b'
@REM echo  %s%
goto:eof

@echo off
title Loading... By El Mano
color 0a
set load=
set/a loadnum=0

:Loading
@REM set load=%load%??
set load=%load%лл
cls
echo.
echo Loading... Please Wait...
echo ----------------------------------------
echo %load%
echo ----------------------------------------
ping localhost -n 2 >nul

set/a loadnum=%loadnum% +1
if %loadnum%==20 goto Done

goto Loading
:Done
echo.
@REM pause

exit