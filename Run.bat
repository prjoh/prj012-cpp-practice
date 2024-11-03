@echo off
setlocal

set PROJECT_DIR=%~dp0
cd /d "%PROJECT_DIR%"

set BUILD_TYPE=Debug
set CALL_CLEAN=0
set CALL_MAKE=0

:parse_args
if "%~1"=="" goto args_parsed
if /I "%~1"=="clean" set CALL_CLEAN=1
if /I "%~1"=="make" set CALL_MAKE=1
if /I "%~1"=="Debug" set BUILD_TYPE=Debug
if /I "%~1"=="Release" set BUILD_TYPE=Release
if /I "%~1"=="MinSizeRel" set BUILD_TYPE=MinSizeRel
if /I "%~1"=="RelWithDebInfo" set BUILD_TYPE=RelWithDebInfo
shift
goto parse_args

:args_parsed
if /I not "%BUILD_TYPE%"=="Debug" if /I not "%BUILD_TYPE%"=="Release" if /I not "%BUILD_TYPE%"=="MinSizeRel" if /I not "%BUILD_TYPE%"=="RelWithDebInfo" (
    echo Error: Invalid BUILD_TYPE "%BUILD_TYPE%". Allowed values are Debug, Release, MinSizeRel, RelWithDebInfo.
    goto end
)

if "%CALL_CLEAN%"=="1" (
    call Clean.bat
)

if "%CALL_MAKE%"=="1" (
    call Make.bat %BUILD_TYPE%
)

set EXECUTABLE_NAME=Game.exe
set BUILD_DIR=Build
set EXECUTABLE_PATH=%BUILD_DIR%\%BUILD_TYPE%\Bin\%EXECUTABLE_NAME%

if exist "%EXECUTABLE_PATH%" (
    "%EXECUTABLE_PATH%"
) else (
    call Make.bat %BUILD_TYPE%
    
    if exist "%EXECUTABLE_PATH%" (
        "%EXECUTABLE_PATH%"
    ) else (
        echo Error: Build failed or executable still not found.
    )
)

:end
endlocal
