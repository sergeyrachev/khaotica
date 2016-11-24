@echo off
rem Build Flavor Java samples

if "%1" == "clean" goto clean

cd basic
call build.bat batch
cd ..

cd gifprint
call build.bat batch
cd ..

cd gif87a
call build.bat batch
cd ..

cd maptest
call build.bat batch
cd ..

cd polytest
call build.bat batch
cd ..

cd audio
call build.bat batch
cd ..

cd mpeg1ss
call build.bat batch
cd ..

cd mpeg1vs
call build.bat batch
cd ..

cd mpeg1as
call build.bat batch

goto done


:clean
echo Clean up...
cd basic
ren Main.java Main.temp
del/q *.java *.bs *.xml basic
rmdir basic
ren Main.temp Main.java
cd ..

cd gifprint
ren Main.java Main.temp
del/q *.java *.bs *.xml gifprint
rmdir gifprint
ren Main.temp Main.java
cd ..

cd gif87a
ren Main.java Main.temp
del/q *.java *.bs *.xml gif87a
rmdir gif87a
ren Main.temp Main.java
cd ..

cd maptest
ren Main.java Main.temp
del/q *.java *.bs *.xml maptest
rmdir maptest
ren Main.temp Main.java
cd ..

cd polytest
ren Main.java Main.temp
del/q *.java *.bs *.xml polytest
rmdir polytest
ren Main.temp Main.java
cd ..

cd audio
ren Main.java Main.temp
del/q *.java *.bs *.xml audio
rmdir audio
ren Main.temp Main.java
cd ..

cd mpeg1ss
ren Main.java Main.temp
del/q *.java *.bs *.xml mpeg1ss
rmdir mpeg1ss
ren Main.temp Main.java
cd ..

cd mpeg1vs
ren Main.java Main.temp
del/q *.java *.bs *.xml mpeg1vs
rmdir mpeg1vs
ren Main.temp Main.java
cd ..

cd mpeg1as
ren Main.java Main.temp
del/q *.java *.bs *.xml mpeg1as
rmdir mpeg1as
ren Main.temp Main.java
cd ..

goto done

:done
pause

