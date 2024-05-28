set(target_name wrl)

add_library(${target_name} STATIC
  wrl/wrl.hpp
  wrl/wrl.cpp
  wrl/tic.hpp
  wrl/tic.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
)
