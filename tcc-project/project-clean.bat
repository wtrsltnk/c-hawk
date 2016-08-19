@echo off
setlocal EnableDelayedExpansion

for %%i in ("%~dp0..") do (
    set "PROJECT_NAME=%%~ni"
    set "PROJECT_ROOT=%%~dpni"
    call %%~di
    cd %%~dpni
)

set OBJ_DIR=%PROJECT_ROOT%\tccproject\obj
set BIN_DIR=%PROJECT_ROOT%\tccproject\bin

echo.
echo @echo Cleaning project %PROJECT_NAME%>%PROJECT_ROOT%\tccproject\temp_clean.bat

if exist %OBJ_DIR% (
    @rem Delete all objects
    echo @echo.>>%PROJECT_ROOT%\tccproject\temp_clean.bat
    echo @echo Cleaning compiled objects:>>%PROJECT_ROOT%\tccproject\temp_clean.bat
    for /f "tokens=*" %%a in (%PROJECT_ROOT%\project-files.txt) do (
        echo @del /f %OBJ_DIR%\%%~na.o>>%PROJECT_ROOT%\tccproject\temp_clean.bat
    )
)

if exist %BIN_DIR% (
    @rem Delete the exe
    echo @echo.>>%PROJECT_ROOT%\tccproject\temp_clean.bat
    echo @echo Cleaning executable:>>%PROJECT_ROOT%\tccproject\temp_clean.bat
    echo @del /f %BIN_DIR%\%PROJECT_NAME%.exe>>%PROJECT_ROOT%\tccproject\temp_clean.bat
)

echo @echo.>>%PROJECT_ROOT%\tccproject\temp_clean.bat
echo @echo All done.>>%PROJECT_ROOT%\tccproject\temp_clean.bat

@echo on
@call %PROJECT_ROOT%\tccproject\temp_clean.bat
@echo off
@del %PROJECT_ROOT%\tccproject\temp_clean.bat

goto :exit 

:exit
