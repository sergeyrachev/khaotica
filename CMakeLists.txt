project(khaotica)
cmake_minimum_required(VERSION 3.9)

find_package(gtest CONFIG PATHS deps)
find_package(boost CONFIG PATHS deps)
find_package(BISON)
find_package(FLEX)

set(GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GRAMMAR_DIR})

find_package(BISON 3.0 REQUIRED)
set(BisonOutput grammar_mpeg2_parser.cpp)
set(BisonOutputH grammar_mpeg2_parser.hpp)
set(BisonInput src/grammar_mpeg2_parser.yy)
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

find_package(FLEX REQUIRED)
set(FlexOutput grammar_mpeg2_lexer.cpp)
set(FlexInput src/grammar_mpeg2_lexer.ll)
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

add_custom_target(dummy
    echo "Dummy target to show Bison/Flex/Flavor files in projects"
    SOURCES
    ${FlexInput}
    ${BisonInput}
    res/test.fl
    res/test.bin
    ${CMAKE_CURRENT_LIST}
    flex/FlexLexer.h

    )

add_executable(khaotica src/khaotica.cpp)
target_sources(khaotica PRIVATE
    ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
    ${GRAMMAR_DIR}/${FlexOutput} src/grammar_mpeg2_lexer.h
    src/grammar_mpeg2_reader.cpp src/grammar_mpeg2_reader.h
    src/grammar_mpeg2_syntax.cpp src/grammar_mpeg2_syntax.h
    src/eval_default.cpp src/eval_default.h
    src/eval_conditional.cpp src/eval_conditional.h
    src/eval_logical.cpp src/eval_logical.h
    src/eval_comparison.cpp src/eval_comparison.h
    src/eval_equality.cpp src/eval_equality.h
    src/eval_bitwise.cpp src/eval_bitwise.h
    src/eval_arithmetic.cpp src/eval_arithmetic.h
    src/grammar_mpeg2_sax.cpp src/grammar_mpeg2_sax.h
    src/logging.cpp src/logging.h
    src/options.cpp src/options.h
    src/interpreter.cpp src/interpreter.h
    src/grammar_mpeg2_types.h src/grammar_mpeg2_types.cpp
    src/bitreader.cpp src/bitreader.h
    src/type_conversion.h
    src/printer.h src/printer.cpp
    )


target_compile_features(khaotica PUBLIC cxx_std_17)
target_link_libraries(khaotica ${boost_LIBRARIES})
target_include_directories(khaotica
    PRIVATE src/
    PRIVATE flex/
    PRIVATE ${boost_INCLUDE_DIRECTORIES}
    PRIVATE ${GRAMMAR_DIR})
target_compile_definitions(khaotica PRIVATE ${boost_COMPILE_DEFINITIONS})

install(TARGETS khaotica RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib/static)
install(DIRECTORY res DESTINATION share/khaotica)

add_executable(khaotica_gtest test/dummy.cpp
    ${GRAMMAR_DIR}/${BisonOutput} ${GRAMMAR_DIR}/${BisonOutputH}
    ${GRAMMAR_DIR}/${FlexOutput} src/grammar_mpeg2_lexer.h
    src/logging.cpp src/logging.h

    test/ut_bitreader.cpp
    test/ut_bit.cpp
    test/ut_parser.cpp)
target_compile_features(khaotica_gtest PUBLIC cxx_std_17)
target_link_libraries(khaotica_gtest
    ${boost_LIBRARIES}
    ${GTEST_LIBRARIES}
    )
target_include_directories(khaotica_gtest
    PRIVATE src/
    PRIVATE test/
    PRIVATE flex/
    PRIVATE ${GTEST_INCLUDE_DIRECTORIES}
    PRIVATE ${boost_INCLUDE_DIRECTORIES}
    PRIVATE ${GRAMMAR_DIR})
target_compile_definitions(khaotica_gtest PRIVATE ${boost_COMPILE_DEFINITIONS})

if (UNIX)

    configure_file("${CMAKE_CURRENT_LIST_DIR}/pvs.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/pvs.cfg" @ONLY)

    include(deps/PVS-Studio.cmake)
    pvs_studio_add_target(TARGET analyze
        OUTPUT FORMAT errorfile
        LOG "report.plog"
        ANALYZE khaotica
        CONFIG "${CMAKE_CURRENT_BINARY_DIR}/pvs.cfg")
endif (UNIX)
