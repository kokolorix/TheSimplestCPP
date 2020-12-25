@echo off
@echo %cd% %*

REM type "%~1"
REM copy "%~dp0cpp-code-runner.h"+"%~1" "%~1.cpp"
REM del "%~1.tmp"
REM ren "%~1.tmp" "%~1"
type "%~dp0cpp-code-runner.h">"%~1.cpp"
type "%~1">>"%~1.cpp"
type "%~1.cpp"

g++ -std=c++11 "%~1.cpp" -o %~n1.exe
REM call %~n1.exe