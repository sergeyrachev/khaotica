Here's a branch specific shield:

```{r, echo=FALSE, eval=TRUE, results="asis"}
travis_url <- "https://travis-ci.org/sergeyrachev/khaotica.svg?branch="
shield <- paste0("[![Build Status](",
                 travis_url,
                 system("git rev-parse --abbrev-ref HEAD", intern = TRUE),
                 ")](https://travis-ci.org/sergeyrachev/khaotica)")
cat(shield)
```

[![Build Status](https://travis-ci.org/sergeyrachev/khaotica.svg?branch=master)](https://travis-ci.org/sergeyrachev/khaotica)

# Khaotica

FLAVOR-inspired bitstream processing tool.

See more about FLAVOR:
http://flavor.sourceforge.net/

#### Preparation:

_Cmake >= 3.6_

Project build is cmake-based and rely on prebuilt LLVM and Boost libraries. I prefer out-sourcetree build. All commands below are executed from temporal build directory.

_LLVM == 3.9_

I started with Exception Handling and RTTI enabled LLVM build. 
		
	cmake -DCMAKE_INSTALL_PREFIX=<LLVM_INSTALLATION_DIRECTORY> -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON  ../llvm 
	cmake --build . --target install
   
_Boost >= 1.60_
	
Let's assume Boost is unpacked and build with b2 in <BOOST_DIRECTORY>. Usually, <BOOST_DIRECTORY> equal .../boost_1_63_0/
	
	bootstrap && b2 -j12 stage

#### Build:
	
Currently only 32bit build Unix\Windows supported out-of-the-box. Pull\Merge requests are welcome.

    cmake -G "Visual Studio 14 2015" -DLLVM_ROOT=d:/work/env/llvm/static/x86/release -DBOOST_ROOT=d:/work/env/boost/boost_1_63_0 ..\khaotica\
#### Usage:

	Under construction...
