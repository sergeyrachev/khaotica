project(khaotica)
cmake_minimum_required(VERSION 3.5)

find_package(llvm CONFIG PATHS deps/llvm)
find_package(boost CONFIG PATHS deps/boost)

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
        src/parsing/flex_lexer.cc
        src/parsing/grammar.tab.cc
        src/parsing/lexer.cc
        src/parsing/tree.cc
        src/codegen/renderer.cc
        src/errors.cc
        )

add_executable(khaotica src/khaotica.cpp ${SOURCES})
set_property(TARGET khaotica PROPERTY CXX_STANDARD 11)

target_link_libraries(khaotica ${llvm_LIBRARIES} ${boost_LIBRARIES})
target_include_directories(khaotica PRIVATE src/ast src/parsing src/codegen src PRIVATE ${llvm_INCLUDE_DIRECTORIES} ${boost_INCLUDE_DIRECTORIES})
target_compile_definitions(khaotica PRIVATE ${llvm_COMPILE_DEFINITIONS} ${boost_COMPILE_DEFINITIONS})
