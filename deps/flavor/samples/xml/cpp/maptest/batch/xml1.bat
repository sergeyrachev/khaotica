@echo off
echo Generate an XML document from a given bitstream . . .

if "%FLAVOR_HOME%" == "" goto noflavorhome

set _FLAVOR_XML_BIN=%FLAVOR_HOME%\samples\xml\bin

set _IN_FILENAME=%FLAVOR_HOME%\samples\data\maptest.bs
set _OUT_FILENAME=maptest1.xml
set _EXEC=maptest

%_FLAVOR_XML_BIN%\%_EXEC% -ofile=%_OUT_FILENAME% -schema=n %_IN_FILENAME%

goto done

:noflavorhome
echo No FLAVOR_HOME environment variable set
goto done

:done
if not "%1" == "batch" pause
