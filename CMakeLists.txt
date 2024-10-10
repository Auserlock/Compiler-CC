cmake_minimum_required(VERSION 3.15)
project(Compiler-CC)

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(LEXER_DIR ${SOURCE_DIR}/lexer)
set(PARSER_DIR ${SOURCE_DIR}/parser)
message(STATUS "LEXER_DIR: ${LEXER_DIR}")

set(BINARY_DIR bin)
set(LEXER_BIN_DIR ${BINARY_DIR}/lexer)

set(CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/bin")

# add_subdirectory(${LEXER_DIR}/src/flex)
add_subdirectory(${LEXER_DIR})

# install(TARGETS lexer
#         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lexer)

install(TARGETS main
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

add_custom_target(clean-file
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*
    COMMENT "Removing all files in the build directory"
)




