set(target_name rational)

add_library(${target_name} STATIC
  rational/rational.hpp
  rational/rational.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
    /usr/include/smucalc/rational/
)
