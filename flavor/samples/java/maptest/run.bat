@echo off
rem Run sample

if "%FLAVOR_HOME%" == "" goto noflavorhome

set _FLAVOR_CLASS=%FLAVOR_HOME%\lib\java
set _FLAVOR_DATA=%FLAVOR_HOME%\samples\data
set _PACKAGE=maptest

set CMD=java -classpath .;%_FLAVOR_CLASS%

set MAIN_CLASS_NAME=%_PACKAGE%.Main
set ARG=

echo %CMD% %MAIN_CLASS_NAME% %ARG%
%CMD% %MAIN_CLASS_NAME% %ARG%
goto done

:noflavorhome
echo No FLAVOR_HOME environment variable set
goto done

:done
pause
