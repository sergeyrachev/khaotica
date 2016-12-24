project(khaotica)
cmake_minimum_required(VERSION 3.0)

include(conanbuildinfo.cmake)
conan_basic_setup()

add_executable(khaotica src/khaotica.cpp)
set_property(TARGET khaotica PROPERTY CXX_STANDARD 11)
target_link_libraries(khaotica ${CONAN_LIBS})
