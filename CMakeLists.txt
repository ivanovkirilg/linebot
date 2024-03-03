cmake_minimum_required(VERSION 3.14)

project(linebot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INC_PATH})

add_library(
    ${PROJECT_NAME}_lib STATIC
    ${INC_PATH}/Controller.hpp
    ${INC_PATH}/Driver.hpp
    ${INC_PATH}/Move.hpp
    ${INC_PATH}/UserInterface.hpp

    ${SRC_PATH}/Controller.cpp
    ${SRC_PATH}/Driver.cpp
    ${SRC_PATH}/UserInterface.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SRC_PATH}/main.cpp
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_lib
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}_lib PRIVATE Threads::Threads)

enable_testing()

add_subdirectory(test)
