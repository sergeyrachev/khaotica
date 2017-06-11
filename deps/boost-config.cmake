if(NOT BOOST_ROOT)
message("Set BOOST_ROOT to BOOST root folder")
endif()

#set(Boost_DEBUG ON)
SET(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Threads REQUIRED)

find_package(Boost 1.60 COMPONENTS system log log_setup thread date_time filesystem program_options )

if(Boost_FOUND)
    set(boost_COMPILE_DEFINITIONS ${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
    set(boost_LIBRARIES ${Boost_LIBRARIES} Threads::Threads)
    set(boost_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
endif()
