@goto start
:: PATH环境须消除cygwin等shell环境带来的影响
:: ARMLMD_LICENSE_FILE为ARM授权环境变量，必须指定，否则将会在系统默认目录中检索
:: 

:start

@set PRJ_ROOT=%CD%
@set ARMLMD_LICENSE_FILE=

@for /f "tokens=*" %%i in ('where python') do @(set "python_path=%%i" & goto :found)

:found
@PATH=C:\Windows;C:\Windows\system32;C:\Windows\system32\Wbem;C:\Windows\system32\WindowsPowerShell;
@for %%I in ("%python_path%") do @set "py_path=%%~dpI"

:: 环境变量配置
@call :add_path %py_path%
@call :add_path %py_path%scripts
@call :add_path %PRJ_ROOT%\tools\win
@call :add_path %PRJ_ROOT%\tools\win\prepass\Hermon\Release
@call :add_path %PRJ_ROOT%\tools\scripts
@call :add_path %PRJ_ROOT%\tools\toolchain\gcc-arm-none-eabi\bin

@set BUILD_CMD=all

::命令处理
@if /i "%1" == "ML307C" (
    @set TARGET_NAME=ML307C
)else if /i "%1" == "CLEAN" (
    @set BUILD_CMD=clean
)else if /i "%1" == "help" (
    @echo usage: kernel_build.bat target
    @echo     target: ML307C CLEAN
    @exit /B
)else (
    @echo param1 error
    @exit /B
)


:: 执行编译
@gnumake  -f kernel\Makefile %BUILD_CMD% PRJ_DIR=%PRJ_ROOT% TARGET_NAME=%TARGET_NAME%


:add_path
@(echo ";%PATH%;" | find /C /I ";%1;" > nul) || set "PATH=%1;%PATH%"
@goto :eof
