project( khaotica )
cmake_minimum_required( VERSION 3.6 )

find_package( gtest CONFIG PATHS deps )
find_package( boost CONFIG PATHS deps )
find_package( BISON )
find_package( FLEX )

set( GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated )
file( MAKE_DIRECTORY ${GRAMMAR_DIR} )

find_package( BISON 3.0 REQUIRED )
set( BisonOutput parser.cpp )
set( BisonOutputH parser.hpp )
set( BisonInput src/parser.yy )
add_custom_command(
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

find_package( FLEX REQUIRED )
set( FlexOutput lexer.cpp )
set( FlexInput src/lexer.ll )
add_custom_command(
    DEPENDS ${FlexInput} ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
    OUTPUT ${GRAMMAR_DIR}/${FlexOutput}
    WORKING_DIRECTORY ${GRAMMAR_DIR}
    COMMAND ${FLEX_EXECUTABLE}
    ARGS
    --outfile=${FlexOutput}
    -L
    ${CMAKE_CURRENT_LIST_DIR}/${FlexInput}
)

add_custom_target( dummy
    echo "Dummy target to show Bison/Flex/Flavor files in projects"
    SOURCES
    ${FlexInput}
    ${BisonInput}
    res/test.fl
    res/test.bin
    ${CMAKE_CURRENT_LIST}
    flex/FlexLexer.h

    )

add_executable( khaotica src/khaotica.cpp src/eval.cpp src/eval.h)
target_sources( khaotica PRIVATE
    ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
    ${GRAMMAR_DIR}/${FlexOutput} src/lexer.h

    src/parser.cpp src/parser.h

    src/logging.cpp src/logging.h
    src/options.cpp src/options.h
    src/interpreter.cpp src/interpreter.h
    src/syntax.h src/syntax.cpp
    src/bitreader.cpp src/bitreader.h
    src/bit.h
    src/printer.h src/printer.cpp
    )


target_compile_features( khaotica PUBLIC cxx_std_17 )
target_link_libraries( khaotica ${boost_LIBRARIES} )
target_include_directories( khaotica
    PRIVATE src/
    PRIVATE flex/
    PRIVATE ${boost_INCLUDE_DIRECTORIES}
    PRIVATE ${GRAMMAR_DIR} )
target_compile_definitions( khaotica PRIVATE ${boost_COMPILE_DEFINITIONS} )

install( TARGETS khaotica RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib/static )
install( DIRECTORY res DESTINATION share/khaotica )

add_executable( khaotica_gtest test/dummy.cpp
    ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
    ${GRAMMAR_DIR}/${FlexOutput} src/lexer.h
    src/logging.cpp src/logging.h

    test/ut_bitreader.cpp
    test/ut_bit.cpp
    test/ut_parser.cpp )
target_compile_features( khaotica_gtest PUBLIC cxx_std_17 )
target_link_libraries( khaotica_gtest
    ${boost_LIBRARIES}
    ${GTEST_LIBRARIES}
     )
target_include_directories( khaotica_gtest
    PRIVATE src/
    PRIVATE test/
    PRIVATE flex/
    PRIVATE ${GTEST_INCLUDE_DIRECTORIES}
    PRIVATE ${boost_INCLUDE_DIRECTORIES}
    PRIVATE ${GRAMMAR_DIR} )
target_compile_definitions( khaotica_gtest PRIVATE ${boost_COMPILE_DEFINITIONS} )

target_compile_options(khaotica PRIVATE /std:c++latest)
target_compile_options(khaotica_gtest PRIVATE /std:c++latest)

if ( UNIX )

    configure_file( "${CMAKE_CURRENT_LIST_DIR}/pvs.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/pvs.cfg" @ONLY )

    include( deps/PVS-Studio.cmake )
    pvs_studio_add_target( TARGET analyze
        OUTPUT FORMAT errorfile
        LOG "report.plog"
        ANALYZE khaotica
        CONFIG "${CMAKE_CURRENT_BINARY_DIR}/pvs.cfg" )
endif ( UNIX )
