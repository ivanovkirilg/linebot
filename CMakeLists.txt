cmake_minimum_required(VERSION 3.14)

project(linebot)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.14.0
)

enable_testing()

add_subdirectory(DOMN)
add_subdirectory(CTRL)
add_subdirectory(DRVR)
add_subdirectory(MAIN)
add_subdirectory(UI)
