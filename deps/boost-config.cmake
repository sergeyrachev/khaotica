message(STATUS "Set BOOST_ROOT to BOOST root folder if not found")

#set(Boost_DEBUG ON)
SET(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Threads REQUIRED)
find_package(Boost 1.60 REQUIRED COMPONENTS log log_setup thread date_time filesystem program_options system)

set(boost_COMPILE_DEFINITIONS ${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
set(boost_LIBRARIES ${Boost_LIBRARIES} Threads::Threads)
set(boost_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
