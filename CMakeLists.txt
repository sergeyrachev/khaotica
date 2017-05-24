project(khaotica)
cmake_minimum_required(VERSION 3.6)

add_subdirectory(flavor)

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
        )

add_executable(khaotica src/khaotica.cpp ${SOURCES} )

target_compile_features(khaotica PUBLIC cxx_nullptr cxx_trailing_return_types cxx_auto_type )
target_link_libraries(khaotica ${boost_LIBRARIES})
target_include_directories(khaotica PRIVATE src/ flex/ PRIVATE PRIVATE ${boost_INCLUDE_DIRECTORIES} PRIVATE ${GRAMMAR_DIR})
target_compile_definitions(khaotica PRIVATE ${boost_COMPILE_DEFINITIONS})

install(TARGETS khaotica RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib/static )
install(DIRECTORY res DESTINATION share/khaotica)

add_executable(khaotica_gtest test/dummy.cpp)
target_include_directories(khaotica_gtest PRIVATE src/ test/ ${GTEST_INCLUDE_DIRECTORIES})
target_link_libraries(khaotica_gtest PRIVATE ${GTEST_LIBRARIES} )

