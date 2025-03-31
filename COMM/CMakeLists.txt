project(COMM)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

if(Doxygen_FOUND)
    doxygen_add_docs(COMM_docs)
endif()

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/Connection.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/IWatchable.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Server.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/Socket.hpp
    ${INT_INC_PATH}/Watcher.hpp

    ${SRC_PATH}/Connection.cpp
    ${SRC_PATH}/Server.cpp
    ${SRC_PATH}/Socket.cpp
    ${SRC_PATH}/Watcher.cpp
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
)

target_link_libraries(${PROJECT_NAME} LOGR)

enable_target_warnings(${PROJECT_NAME})

add_subdirectory(test)
