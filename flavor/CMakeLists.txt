cmake_minimum_required(VERSION 3.6)

find_package(BISON)
find_package(FLEX)

set(GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GRAMMAR_DIR})

FIND_PACKAGE(BISON 3.0 REQUIRED)
set(BisonOutput bison_parser.c)
ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/flavorc/parser.y
        OUTPUT ${GRAMMAR_DIR}/${BisonOutput}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${BISON_EXECUTABLE}
        ARGS
        --defines=y.tab.h
        --output=${BisonOutput}
        ${CMAKE_CURRENT_LIST_DIR}/src/flavorc/parser.y
)

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput flex_lexer.c)
ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/flavorc/lexer.l ${GRAMMAR_DIR}/${BisonOutput}
        OUTPUT ${GRAMMAR_DIR}/${FlexOutput}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${FLEX_EXECUTABLE}
        ARGS
        --outfile=${FlexOutput}
        ${CMAKE_CURRENT_LIST_DIR}/src/flavorc/lexer.l
)

add_executable(flavor
        src/flavorc/symbol.c
        src/flavorc/cclass.c
        src/flavorc/ctrace.c
        src/flavorc/cgetput.c
        src/flavorc/cbac.c
        src/flavorc/cout.c
        src/flavorc/cmap.c
        src/flavorc/cpragma.c
        src/flavorc/main.c
        src/flavorc/eval.c
        src/flavorc/ptree.c
        src/flavorc/cxml.c
        src/flavorc/cverbatim.c
        src/flavorc/cexpr.c
        src/flavorc/compile.c
        src/flavorc/cvar.c
        src/flavorc/check.c
        src/flavorc/cid.c
        src/flavorc/carray.c
        src/flavorc/error.c
        src/flavorc/util.c
        
        ${GRAMMAR_DIR}/${BisonOutput}
        ${GRAMMAR_DIR}/${FlexOutput}
)

target_include_directories(flavor PRIVATE include PRIVATE ${GRAMMAR_DIR} PRIVATE src/flavorc )
