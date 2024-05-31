set(target_name syntaxer)

add_library(${target_name} STATIC
  syntaxer/syntaxer.hpp
  syntaxer/syntaxer.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
)

target_link_libraries(
  ${target_name}
  PRIVATE
    lexer
    ldebug
)
