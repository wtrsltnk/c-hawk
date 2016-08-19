@echo off

for %%i in ("%~dp0.") do set "PROJECT_NAME=%%~ni"

echo REM Build file for %PROJECT_NAME%>temp_build.bat

set OBJ_DIR=obj
if not exist %OBJ_DIR% mkdir %OBJ_DIR%
set BIN_DIR=bin
if not exist %BIN_DIR% mkdir %BIN_DIR%

if not exist tcc_config.bat (
    call find_tcc.bat
)

if not exist tcc_config.bat goto :tcc_not_found

call tcc_config.bat

if not defined TCC goto :tcc_not_found

echo Building %PROJECT_NAME% with %TCC%

set INCLUDES=-I%TCC_INCLUDE%
for /f "tokens=*" %%a in (includes.txt) do (
  set INCLUDES=%INCLUDES% -I%%~a
)

for /f "tokens=*" %%a in (files.txt) do (
    echo %TCC% -c -o %OBJ_DIR%\%%~na.o %%~nxa %INCLUDES%>>temp_build.bat
)

echo|set /p=%TCC% -o %BIN_DIR%\%PROJECT_NAME%.exe>>temp_build.bat
for /f "tokens=*" %%a in (files.txt) do (
    echo|set /p=%OBJ_DIR%\%%~na.o>>temp_build.bat
)

call temp_build.bat
del temp_build.bat

copy *.dll %BIN_DIR%\

echo Done building %PROJECT_NAME%
pause

goto :exit 

:tcc_not_found
pause

:exit