@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "PS_SCRIPT=%SCRIPT_DIR%make_fota_package.ps1"

if not exist "%PS_SCRIPT%" (
    echo PowerShell script not found: %PS_SCRIPT%
    echo.
    pause
    exit /b 1
)

echo Running script: %PS_SCRIPT%
echo.

powershell -NoProfile -ExecutionPolicy Bypass -File "%PS_SCRIPT%"

set "EXIT_CODE=%ERRORLEVEL%"
if not "%EXIT_CODE%"=="0" (
    echo.
    echo make_fota_package failed with exit code %EXIT_CODE%.
)

echo.
pause
exit /b %EXIT_CODE%
