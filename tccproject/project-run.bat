@echo off
setlocal EnableDelayedExpansion

for %%i in ("%~dp0..") do (
    set "PROJECT_NAME=%%~ni"
    set "PROJECT_ROOT=%%~dpni"
    call %%~di
    cd %%~dpni
)

set BIN_DIR=%PROJECT_ROOT%\tccproject\bin
if not exist %BIN_DIR% goto :no_binary_found

call %BIN_DIR%\%PROJECT_NAME%.exe
goto :exit 

:no_binary_found
echo There is no project binary, run tccproject\project-build.bat to build your project

:exit
