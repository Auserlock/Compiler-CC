cmake_minimum_required(VERSION 3.15)
project(Compiler-CC)

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR ../src)
set(LEXER_DIR ${SOURCE_DIR}/lexer)
set(PARSER_DIR ${SOURCE_DIR}/parser)

set(BINARY_DIR ../bin)
set(LEXER_BIN_DIR ${BINARY_DIR}/lexer)

set(CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/bin")

find_program(FLEX_EXECUTABLE flex)
if (NOT FLEX_EXECUTABLE)
    message(FATAL_ERROR "Flex not found!")
endif()

set(FLEX_OUTPUT ${LEXER_DIR}/lex.yy.cc)

add_custom_command(
    OUTPUT ${FLEX_OUTPUT}
    COMMAND ${FLEX_EXECUTABLE} --c++ -o ${FLEX_OUTPUT} ${LEXER_DIR}/lex.l
    DEPENDS ${LEXER_DIR}/lex.l
    COMMENT "Generating lexer from lex.l"
)

add_executable(lexer ${FLEX_OUTPUT} ${LEXER_DIR}/token.hpp)

install(TARGETS lexer
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lexer)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*
    COMMENT "Removing all files in the build directory"
)




