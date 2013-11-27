::
:: This batch file is used to list cpp and include dir
::
@echo off

::�����Ƿ���ʾ������Ϣ
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

::��ȡҪĿ¼���ַ�����
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
::��str1���Ƶ�str
:next1
::��ǩ������goto��ת
::ע����ע����������ע��������ð�ſ�ͷ����ǩ��Ϊһ��ð��
if not "%str%"=="" (
::�ж�str�ǲ��ǿմ������������ִ���±ߵ����
set /a num+=1
::�������㣬ʹnum��ֵ����1���൱��num++����++num���
set "str=%str:~1%"
::��ȡ�ַ�������������
goto next1
::��ת��next1��ǩ
::��������goto�ͱ�ǩ������ѭ���ṹ
)
::%trace% %1 �ĳ���Ϊ��%num%
set %2=%num%
goto :eof

:end
