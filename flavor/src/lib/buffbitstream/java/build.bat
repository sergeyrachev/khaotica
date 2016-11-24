@ECHO OFF
rem Build Flavor Java Run-Time Library
rem
rem build.bat

IF "%FLAVOR_HOME%" == "" goto noflavorhome

set _FLAVOR_CLASS=%FLAVOR_HOME%\lib\JAVA
set CMD=javac -classpath %_FLAVOR_CLASS% -d %_FLAVOR_CLASS%

echo Building Flavor Java Run-Time Library
cd %FLAVOR_HOME%\src\lib\buffbitstream\java
%CMD% *.java
goto done

:noflavorhome
echo No FLAVOR_HOME environment variable is set

:done
pause
