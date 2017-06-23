project(khaotica)
cmake_minimum_required(VERSION 3.6)

find_package(gtest CONFIG PATHS deps)
find_package(boost CONFIG PATHS deps)
find_package(BISON)
find_package(FLEX)

set(GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GRAMMAR_DIR})

FIND_PACKAGE(BISON 3.0 REQUIRED)
set(BisonOutput parser.cpp)
set(BisonOutputH parser.hpp)
set(BisonInput src/parser.y)
ADD_CUSTOM_COMMAND(
        DEPENDS ${BisonInput}
        OUTPUT ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${BISON_EXECUTABLE}
        ARGS
            --output=${BisonOutput}
            --defines=${BisonOutputH}
			--graph
			${CMAKE_CURRENT_LIST_DIR}/${BisonInput}
)

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput scanner.cpp)
set(FlexInput src/lexer.l)
ADD_CUSTOM_COMMAND(
        DEPENDS ${FlexInput} ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
        OUTPUT ${GRAMMAR_DIR}/${FlexOutput}
        WORKING_DIRECTORY ${GRAMMAR_DIR}
        COMMAND ${FLEX_EXECUTABLE}
        ARGS
            --outfile=${FlexOutput}
            -L
			${CMAKE_CURRENT_LIST_DIR}/${FlexInput}
)

add_custom_target(dummy 
	echo "Dummy target to show Bison/Flex/Flavor files in projects"
    SOURCES
        ${FlexInput}
        ${BisonInput}
        res/test.fl
    	res/test.bin
    	${CMAKE_CURRENT_LIST}
)

set(SOURCES
        ${GRAMMAR_DIR}/${BisonOutput}
        ${GRAMMAR_DIR}/${FlexOutput}

        src/renderer.cc src/renderer.h

        src/logging.cpp src/logging.h
		src/options.cpp src/options.h
		src/interpreter.cpp src/interpreter.h

        src/flavorc/carray.cpp
        src/flavorc/carray.h
        src/flavorc/cbac.cpp
        src/flavorc/cbac.h
        src/flavorc/cclass.cpp
        src/flavorc/cclass.h
        src/flavorc/cdef.h
        src/flavorc/cexpr.cpp
        src/flavorc/cexpr.h
        src/flavorc/cgetput.cpp
        src/flavorc/cgetput.h
        src/flavorc/check.cpp
        src/flavorc/check.h
        src/flavorc/cid.cpp
        src/flavorc/cid.h
        src/flavorc/cmap.cpp
        src/flavorc/cmap.h
        src/flavorc/compile.cpp
        src/flavorc/compile.h
        src/flavorc/cout.cpp
        src/flavorc/cout.h
        src/flavorc/cpragma.cpp
        src/flavorc/cpragma.h
        src/flavorc/ctrace.cpp
        src/flavorc/ctrace.h
        src/flavorc/cvar.cpp
        src/flavorc/cvar.h
        src/flavorc/cverbatim.cpp
        src/flavorc/cverbatim.h
        src/flavorc/cxml.cpp
        src/flavorc/cxml.h
        src/flavorc/error.cpp
        src/flavorc/error.h
        src/flavorc/eval.cpp
        src/flavorc/eval.h
        src/flavorc/globals.h
        src/flavorc/ptree.cpp
        src/flavorc/ptree.h
        src/flavorc/symbol.cpp
        src/flavorc/symbol.h
        src/flavorc/util.cpp
        src/flavorc/util.h
        src/flavorc/globals.cpp)

add_executable(khaotica src/khaotica.cpp ${SOURCES} )

target_compile_features(khaotica PUBLIC cxx_nullptr cxx_trailing_return_types cxx_auto_type )
target_link_libraries(khaotica ${boost_LIBRARIES})
target_include_directories(khaotica PRIVATE src/ flex/ PRIVATE ${boost_INCLUDE_DIRECTORIES} PRIVATE ${GRAMMAR_DIR} PRIVATE src/flavorc)
target_compile_definitions(khaotica PRIVATE ${boost_COMPILE_DEFINITIONS})

install(TARGETS khaotica RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib/static )
install(DIRECTORY res DESTINATION share/khaotica)

add_executable(khaotica_gtest test/dummy.cpp)
target_include_directories(khaotica_gtest PRIVATE src/ test/ ${GTEST_INCLUDE_DIRECTORIES})
target_link_libraries(khaotica_gtest PRIVATE ${GTEST_LIBRARIES} )

if (UNIX)

    configure_file("${CMAKE_CURRENT_LIST_DIR}/pvs.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/pvs.cfg" @ONLY)

    include(deps/PVS-Studio.cmake)
    pvs_studio_add_target(TARGET analyze
        OUTPUT FORMAT errorfile
        LOG "report.plog"
        ANALYZE khaotica
        CONFIG "${CMAKE_CURRENT_BINARY_DIR}/pvs.cfg")
endif (UNIX)