@echo off

set OBJ_DIR=obj
set BIN_DIR=bin

if not exist tcc_config.bat (
    call find_tcc.bat
)

if not exist tcc_config.bat goto :tcc_not_found

call tcc_config.bat

if not defined TCC goto :tcc_not_found

if not exist %BIN_DIR% mkdir %BIN_DIR%
if not exist %OBJ_DIR% mkdir %OBJ_DIR%

set INCLUDES=-I%TCC_INCLUDE% -Iscintilla\include\ -I.
%TCC% -c -o %OBJ_DIR%\mainwindow.o mainwindow.c %INCLUDES%
%TCC% -c -o %OBJ_DIR%\editor.o editor.c %INCLUDES%

%TCC% -o %BIN_DIR%\notepad3.exe %OBJ_DIR%\mainwindow.o %OBJ_DIR%\editor.o

copy SciLexer.dll %BIN_DIR%\SciLexer.dll

goto :exit

:tcc_not_found
pause

:exit