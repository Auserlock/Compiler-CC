file(GLOB_RECURSE SOURCE_FILES "${LEXER_DIR}/src/*.cpp")

add_executable(main ${SOURCE_FILES})
target_include_directories(main PRIVATE ${LEXER_DIR}/include)
