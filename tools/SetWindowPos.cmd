<# : cmd part
@echo off & setlocal
cd /d "%~dp0"

@REM type "%~0">"%~dpn0.ps1"
call mklink /h "%~dpn0.ps1" "%~0">nul

call powershell -noprofile -file "%~dpn0.ps1" %*
@REM call erase "%~dpn0.ps1"
goto:end

:end
if "%VSCODE_PID%" equ "" pause
goto:eof
: end batch / begin powershell #>
$signature = @'
[DllImport("user32.dll")]
public static extern bool SetWindowPos(
    IntPtr hWnd,
    IntPtr hWndInsertAfter,
    int X,
    int Y,
    int cx,
    int cy,
    uint uFlags);
'@

$type = Add-Type -MemberDefinition $signature -Name SetWindowPosition -Namespace SetWindowPos -Using System.Text -PassThru
$handle = (Get-Process | Where-Object  {
    $_.MainWindowTitle -imatch '.*Administrator.*'
}).MainWindowHandle;
# $alwaysOnTop = New-Object -TypeName System.IntPtr -ArgumentList (-1)
# $alwaysOnTop = New-Object -TypeName System.IntPtr -ArgumentList (-1)
# $type::SetWindowPos($handle, $alwaysOnTop, 0, 0, 0, 0, 0x0003);
$top = New-Object -TypeName System.IntPtr -ArgumentList (0)
$type::SetWindowPos($handle[2], $top, 100, 100, 1000, 500, 0x0040);

