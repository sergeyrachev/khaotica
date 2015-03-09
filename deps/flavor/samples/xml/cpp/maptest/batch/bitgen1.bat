@echo off
echo Generate bitstream from an XML document (without a schema) . . .

if "%FLAVOR_HOME%" == "" goto noflavorhome

set _FLAVOR_BIN=%FLAVOR_HOME%\bin
set _IN_FILENAME=maptest1.xml
set _OUT_FILENAME=maptest1.bs

%_FLAVOR_BIN%\bitgen -ofile=%_OUT_FILENAME% -schema=n -full=n %_IN_FILENAME%

goto done

:noflavorhome
echo No FLAVOR_HOME environment variable set
goto done

:done
if not "%1" == "batch" pause
