find_program(FLEX_EXECUTABLE flex)
if (NOT FLEX_EXECUTABLE)
    message(FATAL_ERROR "Flex not found!")
endif()

set(FLEX_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.cc)
set(LEX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lex.l)

add_custom_command(
    OUTPUT ${FLEX_OUTPUT}
    COMMAND ${FLEX_EXECUTABLE} --c++ -o ${FLEX_OUTPUT}  ${LEX_FILE}
    DEPENDS ${LEX_FILE}
    COMMENT "Generating lexer from lex.l"
)

add_executable(lexer ${FLEX_OUTPUT})
target_include_directories(lexer PRIVATE ${LEXER_DIR}/include)




