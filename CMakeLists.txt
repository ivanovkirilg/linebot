cmake_minimum_required(VERSION 3.16)

project(linebot)

find_package(Python COMPONENTS Interpreter REQUIRED)

set(cmake_helpers_DIR cmake_helpers)
find_package(cmake_helpers REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/0bdccf4aa2f5c67af967193caf31d42d5c49bde2.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

FetchContent_Declare(
  zpp_bits
  URL https://github.com/eyalz800/zpp_bits/archive/refs/tags/v4.4.25.zip
)
FetchContent_MakeAvailable(zpp_bits)

include_directories(${zpp_bits_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Doxygen QUIET)

add_custom_target(interfaces)

add_subdirectory(SYNC)
add_subdirectory(COMM)
add_subdirectory(DOMN)
add_subdirectory(LOGR)
add_subdirectory(CTRL)
add_subdirectory(DRVR)
add_subdirectory(MAIN)
add_subdirectory(UI)
