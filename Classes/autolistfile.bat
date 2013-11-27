::
:: This batch file is used to list cpp and include dir
::
@echo off

::设置是否显示调试信息
set DEBUG=0

if %DEBUG% == 1 (
   set trace=echo.
   set xcopyprifx=/l
   @echo off
   ) else (
   set trace=rem
   set xcopyprifx=
   @echo off   
 )
setlocal enabledelayedexpansion

set FILE_NAME=file.mk
set DIR_PREFIX=../../Classes

set targetdir=%~dp0
%trace% targetdir is %targetdir%

::获取要目录的字符长度
set lenpath=0
call :strlen "%targetdir%" lenpath
%trace% targetdir's length is %lenpath%

call :filesign
call :cpp-begin
call :cpp-list
call :dir-begin
call :dir-list

echo list-complete!!!
::exit
goto end

:filesign
echo.#  %FILE_NAME% > %FILE_NAME%
echo.#THIS FILE IS AUTO GENERATE BY autolistfile.bat >>%FILE_NAME%
echo.#*DON'T EDIT IT* >>%FILE_NAME%
echo.>>%FILE_NAME%
goto :eof

:cpp-begin
echo.MY_SRC_FILES :=\>> %FILE_NAME%
goto :eof


:cpp-list
for /f "delims=" %%a in ('dir *.cpp;*.c /s /b /a-d 2^>nul') do (
                    call :cpp-write "%%a"
                )
goto :eof

:cpp-write
   %trace% "==========[cpp-write]begin==============="
   %trace% [%1]
   set cpppath=0
   call :filepath %1 cpppath
   %trace% cpppath is "%cpppath%"
   set trspath=%cpppath:\=/%
   %trace% %trspath%
   echo.  %DIR_PREFIX%/%trspath%\>>%FILE_NAME%
goto :eof

:dir-begin
echo.>>%FILE_NAME%
echo.>>%FILE_NAME%
echo.MY_SRC_DIR :=\>>%FILE_NAME%
goto :eof

:dir-list
for /f "delims=" %%a in ('dir /s /b /ad 2^>nul') do (
                    call :dir-write "%%a"
                )
goto :eof

:dir-write
   %trace% "==========[dir-write]begin==============="
   %trace% [%1]
   set dirpath=0
   call :filepath %1 dirpath
   %trace% dirpath is "%dirpath%"
   set trspath=%dirpath:\=/%
   %trace% %trspath%
   echo.  $(LOCAL_PATH)/%DIR_PREFIX%/%trspath%\>>%FILE_NAME%
goto :eof

:filepath
    %trace% filepath %1
    set world=%~1
    set path=!world:~%lenpath%!
    %trace% %path%
    set %2=%path%
goto :eof

:strlen
set num=0
set str=%~1
::将str1复制到str
:next1
::标签，用于goto跳转
::注意与注释语句的区别，注释用两个冒号开头，标签则为一个冒号
if not "%str%"=="" (
::判断str是不是空串，如果不是则执行下边的语句
set /a num+=1
::算术运算，使num的值自增1，相当于num++或者++num语句
set "str=%str:~1%"
::截取字符串，赋给自身
goto next1
::跳转到next1标签
::这里利用goto和标签，构成循环结构
)
::%trace% %1 的长度为：%num%
set %2=%num%
goto :eof

:end
