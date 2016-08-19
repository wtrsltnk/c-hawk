@echo off
setlocal EnableDelayedExpansion

for %%i in ("%~dp0.") do set "PROJECT_NAME=%%~ni"

call %~d0
cd %~dp0

echo REM Build file for %PROJECT_NAME%>temp_build.bat

set OBJ_DIR=obj
if not exist %OBJ_DIR% mkdir %OBJ_DIR%
set BIN_DIR=bin
if not exist %BIN_DIR% mkdir %BIN_DIR%

call tcc-find.bat
if not defined TCC goto :tcc_not_found

echo Building %PROJECT_NAME% with %TCC%

if not exist project-includes.txt echo Replace this line with one or more directories to include in you project>project-includes.txt

set INCLUDES=-I%TCC_INCLUDE%
for /f "tokens=*" %%a in (project-includes.txt) do (
  set INCLUDES=!INCLUDES! -I%%~a
)

if not exist project-files.txt echo Replace this line with one or more source files to include in you project>project-files.txt

for /f "tokens=*" %%a in (project-files.txt) do (
    echo %TCC% -c -o %OBJ_DIR%\%%~na.o %%~a %INCLUDES%>>temp_build.bat
)

echo|set /p=%TCC% -o %BIN_DIR%\%PROJECT_NAME%.exe>>temp_build.bat
for /f "tokens=*" %%a in (project-files.txt) do (
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