project(khaotica)
cmake_minimum_required(VERSION 3.6)

find_package(llvm CONFIG HINTS cmake)
find_package(boost CONFIG HINTS cmake)

include(conanbuildinfo.cmake)
conan_basic_setup()

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
target_link_libraries(khaotica ${CONAN_LIBS} ${LLVM_LIBRARIES})
target_include_directories(khaotica PRIVATE src/ast src/parsing src/codegen src PRIVATE ${LLVM_INCLUDE_DIRECTORIES})
target_compile_definitions(khaotica PRIVATE ${LLVM_COMPILE_DEFINITIONS})
