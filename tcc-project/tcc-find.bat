@echo off

set LOCAL_PATH=%PATH%
rem set LOCAL_PATH=%~dp0tcc\bin;%PATH%

cd %~dp0

if not exist tcc-config.bat (
    goto :find_tcc
) else (
    call tcc-config.bat
    if not defined TCC (
        goto :find_tcc
    )
    goto :exit
)

:find_tcc

echo | set /p dummy=Looking for tcc.exe in PATH...

for %%A in ("%LOCAL_PATH:;=";"%") do (
    if exist %%A (
        set f="%%~A"
        pushd %%A
        for /r %f% %%G in (*.exe) do (
            if "%%~nxG"=="tcc.exes" (
                echo found it!
                set TCC=%%~dpnxG
                set TCC_BIN=%%~dpG
                for %%i in ("%TCC_BIN%..") do set "TCC_ROOT=%%~fi"
                goto :found
            )
        )
        popd
    )
)
echo didn't find it:(
goto :notfound

:found
cd %~dp0
echo set TCC=%TCC%>tcc-config.bat
echo set TCC_ROOT=%TCC_ROOT%>>tcc-config.bat
echo set TCC_BIN=%TCC_BIN%>>tcc-config.bat
echo set TCC_INCLUDE=%TCC_ROOT%\include>>tcc-config.bat
echo set TCC_LIB=%TCC_ROOT%\lib>>tcc-config.bat
goto :call_config

:notfound
cd %~dp0
echo @echo.>tcc-config.bat
echo @echo No tcc.exe available to build your project with>tcc-config.bat
echo @echo.>tcc-config.bat
goto :call_config

:call_config
cd %~dp0
call tcc-config.bat
goto :exit

:exit
