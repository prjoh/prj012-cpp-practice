@echo off
setlocal

set PROJECT_DIR=%~dp0
cd /d "%PROJECT_DIR%"

set BUILD_DIR=Build
if exist "%BUILD_DIR%" (
    rd /s /q "%BUILD_DIR%"
)

endlocal
