set(target_name calc)

add_library(${target_name} STATIC
  calc/calc.hpp
  calc/calc.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
    /usr/include/smucalc/calc/
)

target_link_libraries(
  ${target_name}
  PRIVATE
    rational
    lexer
    ldebug
)
