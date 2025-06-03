set(target_name lexer)

add_library(${target_name} STATIC
  lexer/lexer.hpp
  lexer/lexer.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
    /usr/include/smucalc/lexer/
)
