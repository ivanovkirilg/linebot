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

  set(TEST_NAME "${namespace}::${target}")

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

    set(COVERAGE_TARGET ${namespace}_${target}_cov)
    set(COVERAGE_DIR ${CMAKE_BINARY_DIR}/coverage/${target})

    add_custom_target(
      ${COVERAGE_TARGET}
      lcov --capture
        -d $<TARGET_INTERMEDIATE_DIR:${target}>
        # Line mismatch seems to be an issue with the GTest macros,
        # but TODO should be investigated further
        --ignore-errors inconsistent
        # (intermediate file)
        -o ${target}_lcov.info

      # Filter down to just the component files
      COMMAND lcov --extract
        ${target}_lcov.info ${namespace}
        --ignore-errors inconsistent
        # (intermediate file)
        -o ${target}_${namespace}_lcov.info

      COMMAND genhtml
        ${target}_${namespace}_lcov.info
        --demangle-cpp
        -o ${COVERAGE_DIR}

      COMMAND echo Generated coverage report: ${COVERAGE_DIR}/index.html
    )

    set(COMPONENT_COVERAGE_TARGET ${namespace}_cov)
    if(NOT TARGET ${COMPONENT_COVERAGE_TARGET})
      add_custom_target(${COMPONENT_COVERAGE_TARGET})
      add_dependencies(coverage ${COMPONENT_COVERAGE_TARGET})
    endif()
    add_dependencies(${COMPONENT_COVERAGE_TARGET} ${COVERAGE_TARGET})
  endif()

  add_test(NAME ${TEST_NAME} COMMAND ${target})
endfunction()
