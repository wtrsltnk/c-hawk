@echo off

set LOCAL_PATH=%PATH%
rem set LOCAL_PATH=%~dp0tcc\bin;%PATH%

echo Looking for tcc.exe in PATH...

if not exist tcc_config.bat (
    goto :find_tcc
) else (
    call tcc_config.bat
    if not defined TCC (
        goto :find_tcc
    )
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
cd %~dp0
echo echo Found tcc.exe>tcc_config.bat
echo set TCC=%TCC%>>tcc_config.bat
echo set TCC_ROOT=%TCC_ROOT%>>tcc_config.bat
echo set TCC_BIN=%TCC_BIN%>>tcc_config.bat
echo set TCC_INCLUDE=%TCC_ROOT%\include>>tcc_config.bat
echo set TCC_LIB=%TCC_ROOT%\lib>>tcc_config.bat
goto :call_config

:notfound
echo echo Unable to find tcc.exe>tcc_config.bat
goto :call_config

:call_config
call tcc_config.bat
goto :exit

:exit
