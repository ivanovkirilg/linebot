cmake_minimum_required(VERSION 3.16)

project(linebot)

include(cmake/dependencies.cmake)

include(cmake/generate_docs.cmake)

include(cmake/cpp.cmake)

enable_testing()

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
