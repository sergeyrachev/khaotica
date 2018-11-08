Build for Linux: [![Build Status](https://travis-ci.org/sergeyrachev/khaotica.svg?branch=master)](https://travis-ci.org/sergeyrachev/khaotica)

Build for Windows: [![Build status](https://ci.appveyor.com/api/projects/status/2mgxm4i8c29v1ilw?svg=true)](https://ci.appveyor.com/project/sergeyrachev/khaotica)

# Khaotica

FLAVOR-inspired bitstream processing tool.

See more about FLAVOR:
http://flavor.sourceforge.net/

### Preparation:

* CMake >= 3.6
* Boost >= 1.63
* Bison >= 3.0
* Flex >= 2.6.4

Project supports CMake-based build process and relies on prebuilt Boost libraries. It would be nice to use not in-source build so all commands below are executed from temporal build directory.

### Build:

Pull requests are welcome. There are Travis-CI and Appveyor support in the project dir. You can look at its build scripts in project root folder to solve build issues.
    
    cmake ..
    cmake --build .
      
### Usage:

'res' subfolder contains sample simulated with text file bitstream test.bin and its declaration in test.fl. Execute application as shown below to process "bitstream" accordingly its declaration.
 
    khaotica -i example.fl -I example.bin
   

   
 
