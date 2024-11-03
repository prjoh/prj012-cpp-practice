@echo off
setlocal

cd /d "%~dp0"

rmdir /s /q "VS2022" 2>nul

cmake -B VS2022 -G "Visual Studio 17 2022" -A x64

mklink /D "VS2022\Assets" "%cd%\Assets"

:end
endlocal
