Build for Linux: [![Build Status](https://travis-ci.org/sergeyrachev/khaotica.svg?branch=master)](https://travis-ci.org/sergeyrachev/khaotica)

Build for Windows: [![Build status](https://ci.appveyor.com/api/projects/status/2mgxm4i8c29v1ilw?svg=true)](https://ci.appveyor.com/project/sergeyrachev/khaotica)

# Khaotica

FLAVOR-inspired bitstream processing tool.

See more about FLAVOR:
http://flavor.sourceforge.net/

### Preparation:

* CMake >= 3.9
* Boost >= 1.63

Project supports CMake-based build process and relies on prebuilt Boost libraries. It would be nice to use not in-source build so all commands below are executed from temporal build directory.

**Boost**

Let's assume Boost is unpacked and to /boost_1_63_0/ change working directory into that folder and execute:
	
	./bootstrap.sh && ./b2 -j12 -d0 --prefix=$HOME/deps/boost variant=release threading=multi link=static install
    
OR 
    
	bootstrap.bat && b2 -j12 --prefix=d:\work\env\boost\prebuilt\x64\ --build-dir=d:\work\env\boost\build\x64\ variant=debug,release threading=multi link=static toolset=msvc address-model=64 install
    
OR

	bootstrap.bat && b2 -j12 --prefix=d:\work\env\boost\prebuilt\x86\ --build-dir=d:\work\env\boost\build\x86\ variant=debug,release threading=multi link=static toolset=msvc install

### Build:

Pull requests are welcome. There are Travis-CI and Appveyor support in the project dir. You can look at its build scripts in project root folder to solve build issues.

    cmake -DBOOST_ROOT=$HOME/deps/boost ..\khaotica\
    
    OR
    
    cmake -G "Visual Studio 14 2015 Win64" -DBOOST_ROOT=d:/work/env/boost/prebuilt/x64/ ../khaotica/
   
### Usage:

'res' subfolder contains sample simulated with text file bitstream test.bin and its declaration in test.fl. Execute application as shown below to process "bitstream" accordingly its declaration.
 
    khaotica -i test.fl -I test.bin
   

   
 
