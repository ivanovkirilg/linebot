cmake_minimum_required(VERSION 3.16)

project(linebot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/dependencies.cmake)

enable_testing()

if(Doxygen_FOUND)
  doxygen_add_docs(docs)
endif()

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

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(interfaces)

add_subdirectory(SYNC)
add_subdirectory(COMM)
add_subdirectory(DOMN)
add_subdirectory(LOGR)
add_subdirectory(CTRL)
add_subdirectory(DRVR)
add_subdirectory(MAIN)
add_subdirectory(UI)
