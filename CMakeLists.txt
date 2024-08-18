cmake_minimum_required(VERSION 3.14)

project(linebot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
cmake_policy(SET CMP0135 NEW)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_package(Doxygen QUIET)

add_subdirectory(DOMN)
add_subdirectory(LOGR)
add_subdirectory(CTRL)
add_subdirectory(DRVR)
add_subdirectory(MAIN)
add_subdirectory(UI)
