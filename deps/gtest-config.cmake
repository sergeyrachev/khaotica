
set(gtest_force_shared_crt ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/googletest)
set(GTEST_LIBRARIES gmock_main)
get_target_property(GTEST_INCLUDE_DIRECTORIES gtest INTERFACE_INCLUDE_DIRECTORIES)
