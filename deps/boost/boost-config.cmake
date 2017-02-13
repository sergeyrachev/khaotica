set(BOOST_ROOT d:/work/env/boost/1.60/)
set(BOOST_LIBRARYDIR d:/work/env/boost/1.60/stage/shared/x86/lib)

#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#find_package(Boost 1.60)

if(Boost_FOUND)
    #set(boost_COMPILE_DEFINITIONS ${Boost_LIB_DIAGNOSTIC_DEFINITIONS} -DBOOST_AL)
    #set(boost_LIBRARIES ${Boost_LIBRARIES})
    #set(boost_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
endif()
