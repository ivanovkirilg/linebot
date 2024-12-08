cmake_minimum_required(VERSION 3.14)

project(COMM)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

if(Doxygen_FOUND)
    doxygen_add_docs(${PROJECT_NAME}_docs)
endif()

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/Connection.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/InterfaceProtocol.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/IWatchable.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Server.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Socket.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Watcher.hpp

    ${SRC_PATH}/Connection.cpp
    ${SRC_PATH}/Server.cpp
    ${SRC_PATH}/Socket.cpp
    ${SRC_PATH}/Watcher.cpp
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
)

target_link_libraries(${PROJECT_NAME} LOGR)

add_subdirectory(test)
add_subdirectory(py/interface_generator/example)
