@echo off
where bash >nul 2>nul
if %errorlevel% neq 0 (
    echo(
    echo Error: Bash was not found on your system.
    echo(
    echo This script requires Bash to run the build.sh script.
    echo(
    echo Recommended Option:
    echo   - Git for Windows (includes Git Bash^): https://gitforwindows.com
    echo(
    echo Other options:
    echo   - MSYS2: https://www.msys2.org
    echo   - Cygwin: https://www.cygwin.com
    echo   - WSL: https://learn.microsoft.com/windows/wsl/
    echo(
    echo After installing, make sure "bash" is available in your system PATH.
    exit /b 1
)

call bash ./build.sh %*