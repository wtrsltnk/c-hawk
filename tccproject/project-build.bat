@echo off
setlocal EnableDelayedExpansion

for %%i in ("%~dp0..") do (
    set "PROJECT_NAME=%%~ni"
    set "PROJECT_ROOT=%%~dpni"
    call %%~di
    cd %%~dpni
)

set OBJ_DIR=%PROJECT_ROOT%\tccproject\obj
if not exist %OBJ_DIR% mkdir %OBJ_DIR%
set BIN_DIR=%PROJECT_ROOT%\tccproject\bin
if not exist %BIN_DIR% mkdir %BIN_DIR%

call %PROJECT_ROOT%\tccproject\tcc-find.bat
if not defined TCC goto :tcc_not_found

echo.
echo @echo Building %BIN_DIR%\%PROJECT_NAME%.exe with %TCC%>%PROJECT_ROOT%\tccproject\temp_build.bat

if not exist %PROJECT_ROOT%\project-includes.txt echo Replace this text with one directory per line to include in you project>%PROJECT_ROOT%\project-includes.txt

@rem Gather all configured include directories. These must be relative from project path
set INCLUDES=-I%TCC_INCLUDE%
for /f "tokens=*" %%a in (%PROJECT_ROOT%\project-includes.txt) do (
    if exist %%~a (
        set INCLUDES=!INCLUDES! -I%%~a
    ) else (
        set INCLUDES=!INCLUDES! -I%PROJECT_ROOT%\%%~a
    )
)

if not exist %PROJECT_ROOT%\project-files.txt echo Replace this text with one source file per line to include in you project>%PROJECT_ROOT%\project-files.txt

echo @echo.>>%PROJECT_ROOT%\tccproject\temp_build.bat

@rem Compile all sources to objects
for /f "tokens=*" %%a in (%PROJECT_ROOT%\project-files.txt) do (
    echo @echo Compiling %%~a>>%PROJECT_ROOT%\tccproject\temp_build.bat
    echo @%TCC% -c %INCLUDES% -o %OBJ_DIR%\%%~na.o %PROJECT_ROOT%\%%~a>>%PROJECT_ROOT%\tccproject\temp_build.bat
    echo @if ERRORLEVEL 1 goto :errorHandling>>%PROJECT_ROOT%\tccproject\temp_build.bat
)

@rem Link all objects to one exe
echo @echo.>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo @echo Linking all objects into %BIN_DIR%\%PROJECT_NAME%.exe>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo|set /p=@%TCC% -o %BIN_DIR%\%PROJECT_NAME%.exe>>%PROJECT_ROOT%\tccproject\temp_build.bat
for /f "tokens=*" %%a in (%PROJECT_ROOT%\project-files.txt) do (
    echo | set /p dummy=%OBJ_DIR%\%%~na.o>>%PROJECT_ROOT%\tccproject\temp_build.bat
)

echo.>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo @echo.>>%PROJECT_ROOT%\tccproject\temp_build.bat

@rem When there are binaries configfured, copy these to the bin folder
if exist %PROJECT_ROOT%\project-binaries.txt (
    for /f "tokens=*" %%a in (%PROJECT_ROOT%\project-binaries.txt) do (
        echo copy %%~a %BIN_DIR%>>%PROJECT_ROOT%\tccproject\temp_build.bat
    )
)

echo @echo Done building %PROJECT_NAME%>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo @echo.>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo @goto :exit>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo :errorHandling>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo @echo.>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo @echo Error compiling %PROJECT_NAME%>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo @echo.>>%PROJECT_ROOT%\tccproject\temp_build.bat
echo :exit>>%PROJECT_ROOT%\tccproject\temp_build.bat

@echo on
@call %PROJECT_ROOT%\tccproject\temp_build.bat
@echo off
@del %PROJECT_ROOT%\tccproject\temp_build.bat

goto :exit 

:tcc_not_found
echo We cannot build %PROJECT_NAME%.exe without tcc.exe. Add the location of tcc.exe to your environemnt PATH.

:exit
