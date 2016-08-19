@echo off

set LOCAL_PATH=%PATH%
rem set LOCAL_PATH=%~dp0tcc\bin;%PATH%

cd %~dp0

echo | set /p dummy=Looking for tcc.exe in PATH...

if not exist tcc-config.bat (
    goto :find_tcc
) else (
    call tcc-config.bat
    if not defined TCC (
        goto :find_tcc
    )
    echo found it!
    goto :exit
)

:find_tcc
for %%A in ("%LOCAL_PATH:;=";"%") do (
    if exist %%A (
        set f="%%~A"
        pushd %%A
        for /r %f% %%G in (*.exe) do (
            if "%%~nxG"=="tcc.exe" (
                set TCC=%%~dpnxG
                set TCC_BIN=%%~dpG
                for %%i in ("%TCC_BIN%..") do set "TCC_ROOT=%%~fi"
                goto :found
            )
        )
        popd
    )
)
goto :notfound

:found
echo found it!
cd %~dp0
echo set TCC=%TCC%>tcc-config.bat
echo set TCC_ROOT=%TCC_ROOT%>>tcc-config.bat
echo set TCC_BIN=%TCC_BIN%>>tcc-config.bat
echo set TCC_INCLUDE=%TCC_ROOT%\include>>tcc-config.bat
echo set TCC_LIB=%TCC_ROOT%\lib>>tcc-config.bat
goto :call_config

:notfound
echo didn't find it:(
echo @echo Unable to find tcc.exe>tcc-config.bat
goto :call_config

:call_config
call tcc-config.bat
goto :exit

:exit
