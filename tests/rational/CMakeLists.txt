set(target_name rational.test)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
  rational/rational.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
  gtest_main
  rational
)

include(GoogleTest)
gtest_discover_tests(${target_name})
