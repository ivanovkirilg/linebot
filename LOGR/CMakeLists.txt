cmake_minimum_required(VERSION 3.14)

project(LOGR)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

doxygen_add_docs(docs)

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/Exception.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/internal.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Logger.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Trace.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Warning.hpp

    ${SRC_PATH}/Exception.cpp
    ${SRC_PATH}/internal.cpp
    ${SRC_PATH}/Logger.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${EXT_INC_PATH})

add_subdirectory(test)
