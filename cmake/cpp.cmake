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
