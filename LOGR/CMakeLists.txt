project(LOGR)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

if(Doxygen_FOUND)
    doxygen_add_docs(LOGR_docs)
endif()

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/Exception.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/internal.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/ILogger.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Trace.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Warning.hpp

    ${INT_INC_PATH}/LoggerImpl.hpp

    ${SRC_PATH}/Exception.cpp
    ${SRC_PATH}/internal.cpp
    ${SRC_PATH}/ILogger.cpp
    ${SRC_PATH}/LoggerImpl.cpp
)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${EXT_INC_PATH})

add_subdirectory(test)
