
set(gtest_force_shared_crt ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/googletest)
set(GTEST_LIBRARIES gmock gmock_main gtest)