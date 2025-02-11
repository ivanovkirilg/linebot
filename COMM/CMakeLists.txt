cmake_minimum_required(VERSION 3.14)

project(COMM)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(INTERFACE_PATH ${CMAKE_CURRENT_BINARY_DIR})

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

set(COMM_PYTHONPATH ${CMAKE_CURRENT_SOURCE_DIR}/py CACHE PATH
    "Path to COMM python packages (like interface_generator)")

function(add_interface component interface)
    add_custom_command(
        OUTPUT
            ${INTERFACE_PATH}/${interface}Client.hpp
            ${INTERFACE_PATH}/${interface}Client.cpp
            ${INTERFACE_PATH}/${interface}Server.hpp
            ${INTERFACE_PATH}/${interface}Server.cpp
        COMMAND
            ${CMAKE_COMMAND} -E env "PYTHONPATH=${COMM_PYTHONPATH}"
            ${Python_EXECUTABLE} -m interface_generator -n ${component} -i ${INTERFACE_PATH} -s ${INTERFACE_PATH} ${EXT_INC_PATH}/${PROJECT_NAME}/${interface}.interface
        DEPENDS
            ${EXT_INC_PATH}/${PROJECT_NAME}/${interface}.interface
        VERBATIM
    )
    add_custom_target(
        ${interface}_interface_files
        DEPENDS
            ${INTERFACE_PATH}/${interface}Client.hpp
            ${INTERFACE_PATH}/${interface}Client.cpp
            ${INTERFACE_PATH}/${interface}Server.hpp
            ${INTERFACE_PATH}/${interface}Server.cpp
    )
    add_library(
        ${interface}_interface SHARED
        ${INTERFACE_PATH}/${interface}Client.hpp
        ${INTERFACE_PATH}/${interface}Server.hpp

        ${INTERFACE_PATH}/${interface}Client.cpp
        ${INTERFACE_PATH}/${interface}Server.cpp
    )

    target_link_libraries(${interface}_interface COMM)
endfunction()

add_subdirectory(test)
add_subdirectory(py/interface_generator/example)
