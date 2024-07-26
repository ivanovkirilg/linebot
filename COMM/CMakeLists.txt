cmake_minimum_required(VERSION 3.14)

project(COMM)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/Socket.hpp

    ${SRC_PATH}/Socket.cpp
)

add_executable(
    server
    ${SRC_PATH}/server.cpp
)
target_link_libraries(server PRIVATE ${PROJECT_NAME})

add_executable(
    client
    ${SRC_PATH}/client.cpp
)
target_link_libraries(client PRIVATE ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
)

# target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

add_subdirectory(test)
