project(khaotica)
cmake_minimum_required(VERSION 3.6)

add_subdirectory(flavor)

find_package(gtest CONFIG PATHS deps)
find_package(llvm CONFIG PATHS deps)
find_package(boost CONFIG PATHS deps)
find_package(BISON)
find_package(FLEX)

set(GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GRAMMAR_DIR})

FIND_PACKAGE(BISON 3.0 REQUIRED)
set(BisonOutput parser.cpp)
set(BisonOutputH parser.hpp)
ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/parsing/parser.yy
        OUTPUT ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${BISON_EXECUTABLE}
        ARGS
            --output=${BisonOutput}
            --defines=${BisonOutputH}
            ${CMAKE_CURRENT_LIST_DIR}/src/parsing/parser.yy
)

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput scanner.cpp)
ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/parsing/scanner.ll ${GRAMMAR_DIR}/${BisonOutput}
        OUTPUT ${GRAMMAR_DIR}/${FlexOutput}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${FLEX_EXECUTABLE}
        ARGS
            --outfile=${FlexOutput}
            ${CMAKE_CURRENT_LIST_DIR}/src/parsing/scanner.ll
)

add_custom_target(dummy 
	echo "Dummy target to show Bison/Flex/Flavor files in projects"
    SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/parsing/lexer.ll 
    ${CMAKE_CURRENT_LIST_DIR}/src/parsing/scanner.ll 
    ${CMAKE_CURRENT_LIST_DIR}/src/parsing/grammar.yy
    ${CMAKE_CURRENT_LIST_DIR}/src/parsing/parser.yy
    ${CMAKE_CURRENT_LIST_DIR}/res/test.fl
    ${CMAKE_CURRENT_LIST_DIR}/res/test.bin
    ${CMAKE_CURRENT_LIST}
    
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(export SHARED src/export.cpp src/repository.cpp src/repository.h )
target_include_directories(export PRIVATE src)
target_compile_features(export PUBLIC cxx_nullptr cxx_trailing_return_types cxx_auto_type)

set(SOURCES
        ${GRAMMAR_DIR}/${BisonOutputH}
        ${GRAMMAR_DIR}/${BisonOutput}
        ${GRAMMAR_DIR}/${FlexOutput}
    
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
        src/codegen/renderer.cc
        src/errors.cc

        src/ast/variable.h
        src/ast/var.h
        src/ast/unary.h
        src/ast/prototype.h
        src/ast/number.h
        src/ast/node.h
        src/ast/if.h
        src/ast/function.h
        src/ast/for.h
        src/ast/call.h
        src/ast/binary.h
        
		src/logging.cpp
		src/options.cpp
        src/parsing/interpreter.cpp
        )

add_executable(khaotica src/khaotica.cpp ${SOURCES} )

target_compile_features(khaotica PUBLIC cxx_nullptr cxx_trailing_return_types cxx_auto_type )
target_link_libraries(khaotica ${llvm_LIBRARIES} ${boost_LIBRARIES} export)
target_include_directories(khaotica PRIVATE src/ast src/parsing src/codegen src/ flex/ PRIVATE ${llvm_INCLUDE_DIRECTORIES} PRIVATE ${boost_INCLUDE_DIRECTORIES} PRIVATE ${GRAMMAR_DIR})
target_compile_definitions(khaotica PRIVATE ${llvm_COMPILE_DEFINITIONS} ${boost_COMPILE_DEFINITIONS})

install(TARGETS khaotica export RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib/static )

add_executable(khaotica_gtest test/dummy.cpp)
target_include_directories(khaotica_gtest PRIVATE src/ test/ ${GTEST_INCLUDE_DIRECTORIES})
target_link_libraries(khaotica_gtest PRIVATE ${GTEST_LIBRARIES} )

