project(khaotica)
cmake_minimum_required(VERSION 3.6)

find_package(llvm CONFIG PATHS deps)
find_package(boost CONFIG PATHS deps)
find_package(BISON)
find_package(FLEX)

FIND_PACKAGE(BISON REQUIRED)
SET(BisonOutput ${CMAKE_CURRENT_LIST_DIR}/src/parsing/grammar.cpp)
IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${BisonOutput}
            COMMAND ${BISON_EXECUTABLE}
            --defines=${CMAKE_CURRENT_LIST_DIR}/src/parsing/tokens.h
            --output=${BisonOutput}
            ${CMAKE_CURRENT_LIST_DIR}/src/parsing/grammar.yy
    )
ENDIF()

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput ${CMAKE_CURRENT_LIST_DIR}/src/parsing/lexer.cpp)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            --outfile=${FlexOutput}
            ${CMAKE_SOURCE_DIR}/src/parsing/lexer.ll
    )
ENDIF()


set(SOURCES
        src/ast/binary.cc
        src/ast/call.cc
        src/ast/for.cc
        src/ast/function.cc
        src/ast/if.cc
        src/ast/number.cc
        src/ast/prototype.cc
        src/ast/unary.cc
        src/ast/var.cc
        src/ast/variable.cc
        src/parsing/tree.cc
        src/parsing/lexer.cc
        src/codegen/renderer.cc
        src/errors.cc

        ${BisonOutput}
        ${FlexOutput}
        )

add_executable(khaotica src/khaotica.cpp ${SOURCES})

target_compile_features(khaotica PUBLIC cxx_nullptr)

target_link_libraries(khaotica ${llvm_LIBRARIES} ${boost_LIBRARIES})
target_include_directories(khaotica PRIVATE src/ast src/parsing src/codegen src PRIVATE ${llvm_INCLUDE_DIRECTORIES} ${boost_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_BINARY_DIR})
target_compile_definitions(khaotica PRIVATE ${llvm_COMPILE_DEFINITIONS} ${boost_COMPILE_DEFINITIONS})
