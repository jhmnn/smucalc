set(target_name ldebug)

add_library(${target_name} STATIC
  debug/debug.hpp
  debug/debug.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
)
