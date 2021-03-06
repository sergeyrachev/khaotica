@echo off
echo Generate an XML document from a given bitstream...

if "%FLAVOR_HOME%" == "" goto noflavorhome

set _FLAVOR_XML_BIN=%FLAVOR_HOME%\samples\xml\bin

set _IN_FILENAME=%FLAVOR_HOME%\samples\data\low.gif
set _OUT_FILENAME=low2.xml
set _EXEC=gif87a

%_FLAVOR_XML_BIN%\%_EXEC% -ofile=%_OUT_FILENAME% -schema=y %_IN_FILENAME%

goto done

:noflavorhome
echo No FLAVOR_HOME environment variable set
goto done

:done
if not "%1" == "batch" pause
