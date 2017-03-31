project(khaotica)
cmake_minimum_required(VERSION 3.6)

find_package(llvm CONFIG PATHS deps)
find_package(boost CONFIG PATHS deps)
find_package(BISON)
find_package(FLEX)

set(GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GRAMMAR_DIR})

FIND_PACKAGE(BISON 3.0 REQUIRED)
set(BisonOutput bison_parser.cpp)
ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/parsing/grammar.yy
        OUTPUT ${GRAMMAR_DIR}/${BisonOutput}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${BISON_EXECUTABLE}
        ARGS
            --output=${BisonOutput}
            ${CMAKE_CURRENT_LIST_DIR}/src/parsing/grammar.yy
)

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput flex_lexer.cpp)
ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/parsing/lexer.ll ${GRAMMAR_DIR}/${BisonOutput}
        OUTPUT ${GRAMMAR_DIR}/${FlexOutput}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${FLEX_EXECUTABLE}
        ARGS
            --outfile=${FlexOutput}
            ${CMAKE_CURRENT_LIST_DIR}/src/parsing/lexer.ll
)

add_custom_target(dummy 
	echo "Dummy target to show Bison/Flex/Flavor files in projects"
    SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/parsing/lexer.ll 
    ${CMAKE_CURRENT_LIST_DIR}/src/parsing/grammar.yy
    ${CMAKE_CURRENT_LIST_DIR}/test/test.fl)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(export SHARED src/export.cpp src/repository.cpp src/repository.h )
target_include_directories(export PRIVATE src)
target_compile_features(export PUBLIC cxx_nullptr cxx_trailing_return_types cxx_auto_type)

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
        
        ${GRAMMAR_DIR}/${BisonOutput}
        ${GRAMMAR_DIR}/${FlexOutput}
		src/logging.cpp src/options.cpp src/options.h )

add_executable(khaotica src/khaotica.cpp ${SOURCES} )

target_compile_features(khaotica PUBLIC cxx_nullptr cxx_trailing_return_types cxx_auto_type )
target_link_libraries(khaotica ${llvm_LIBRARIES} ${boost_LIBRARIES} export)
target_include_directories(khaotica PRIVATE src/ast src/parsing src/codegen src/ flex/ PRIVATE ${llvm_INCLUDE_DIRECTORIES} PRIVATE ${boost_INCLUDE_DIRECTORIES} PRIVATE ${GRAMMAR_DIR})
target_compile_definitions(khaotica PRIVATE ${llvm_COMPILE_DEFINITIONS} ${boost_COMPILE_DEFINITIONS})

install(TARGET khaotica export RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib/static )

add_subdirectory(flavor)