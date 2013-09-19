@echo off
IF /I "%1"=="" GOTO ERRO

SET VXSQLITE_VERSION=%1

echo Deleting old files...
del ..\build\obj\lib\*.* /q
del ..\build\obj\tests\*.* /q
del ..\build\out\lib\*.* /q
del ..\build\out\tests\*.* /q

echo Compiling VxSqlite library...
cd ..\mkfile\generic
nmake sqlite.smk
copy ..\..\sqlite\%1\*.h ..\..\build\out\lib

echo Compiling tests...
cd ..
cd ..
cd tests
nmake tests.smk

cd ..\build
echo DONE!!!
goto FIM

:ERRO
echo You should fill up the version. Example: c 3.6.14.1

:FIM