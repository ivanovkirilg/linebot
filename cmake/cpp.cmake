set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(enable_target_warnings target)
  target_compile_options(
    ${target} PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wundef
      -Wuninitialized
      -Wshadow
      -Wunused-parameter
      -Wnull-dereference
  )
endfunction()

function(enable_target_coverage target)
  target_compile_options(${target} PRIVATE -coverage)
  target_link_options(${target} PRIVATE -coverage)
endfunction()

function(add_cpp_test namespace target)
  cmake_parse_arguments(PARSE_ARGV 2 arg
    "NO_COVERAGE" "" "SOURCES;LIBRARIES;INCDIRS"
  )

  add_executable(
    ${target}
    ${arg_SOURCES}
  )
  target_link_libraries(
    ${target}
    ${arg_LIBRARIES}
    GTest::gmock_main
  )
  target_include_directories(
    ${target}
    PRIVATE
    ${arg_INCDIRS}
  )
  enable_target_warnings(${target})

  # Beware: double negative for the sake of a preferable default
  if(${arg_NO_COVERAGE} STREQUAL "FALSE")
    enable_target_coverage(${target})
  endif()

  add_test(NAME ${namespace}::${target} COMMAND ${target})
endfunction()
