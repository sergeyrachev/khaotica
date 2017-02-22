set(BOOST_ROOT /home/sergey.rachev/data/opt/boost/1.63)

find_package(Boost COMPONENTS program_options system)

if(Boost_FOUND)
    set(boost_COMPILE_DEFINITIONS ${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
    set(boost_LIBRARIES ${Boost_LIBRARIES})
    set(boost_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
endif()
