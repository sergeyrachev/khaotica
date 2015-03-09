@echo off
echo Generate an XML document from a given bitstream...

if "%FLAVOR_HOME%" == "" goto noflavorhome

set _FLAVOR_CLASS=%FLAVOR_HOME%\lib\java
set _FLAVOR_XML_BIN=%FLAVOR_HOME%\samples\xml\bin


set _IN_FILENAME=%FLAVOR_HOME%\samples\data\polytest.bs
set _OUT_FILENAME=polytest2.xml
set _PACKAGE=polytest


set _CMD=java -classpath .;%_FLAVOR_CLASS%;%_FLAVOR_XML_BIN% %_PACKAGE%.Main

%_CMD% -ofile=%_OUT_FILENAME% -schema=y %_IN_FILENAME%

goto done

:noflavorhome
echo No FLAVOR_HOME environment variable set
goto done

:done
if not "%1" == "batch" pause
