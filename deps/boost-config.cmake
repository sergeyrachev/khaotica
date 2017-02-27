set(BOOST_ROOT "" CACHE PATH "")
#set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.60 COMPONENTS program_options system date_time thread)

if(Boost_FOUND)
    set(boost_COMPILE_DEFINITIONS ${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
    set(boost_LIBRARIES ${Boost_LIBRARIES})
    set(boost_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
endif()
