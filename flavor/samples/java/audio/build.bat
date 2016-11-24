@echo off
rem Build Flavor Java sample

if "%FLAVOR_HOME%" == "" goto noflavorhome

set _FLAVOR_BIN=%FLAVOR_HOME%\bin
set _FLAVOR_CLASS=%FLAVOR_HOME%\lib\java
set _FLAVOR_FL=%FLAVOR_HOME%\samples\fl
set _FLAVOR_FILENAME=audio.fl

set FLAVOR_CMD=%_FLAVOR_BIN%\flavorc -gj -x %_FLAVOR_FL%\%_FLAVOR_FILENAME%
set CMD=javac -classpath %_FLAVOR_CLASS% -d .

echo Translating the Flavor file...
echo %FLAVOR_CMD%
%FLAVOR_CMD%

echo Compiling Java files...
echo %CMD% *.java
%CMD% *.java
goto done

:noflavorhome
echo No FLAVOR_HOME environment variable set
goto done

:done
if not "%1" == "batch" pause
