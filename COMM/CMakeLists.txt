project(COMM)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(INTERFACE_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${INT_INC_PATH})

if(Doxygen_FOUND)
    doxygen_add_docs(COMM_docs)
endif()

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/Connection.hpp
    ${EXT_INC_PATH}/${PROJECT_NAME}/InterfaceProtocol.hpp
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
            ${Python_EXECUTABLE} -m interface_generator -n ${component} -o ${INTERFACE_PATH} ${EXT_INC_PATH}/${PROJECT_NAME}/${interface}.interface
        DEPENDS
            ${EXT_INC_PATH}/${PROJECT_NAME}/${interface}.interface

            ${COMM_PYTHONPATH}/interface_generator/__main__.py
            ${COMM_PYTHONPATH}/interface_generator/generator.py
            ${COMM_PYTHONPATH}/interface_generator/lexer.py
            ${COMM_PYTHONPATH}/interface_generator/parser.py
            ${COMM_PYTHONPATH}/interface_generator/syntax_tree.py
            ${COMM_PYTHONPATH}/interface_generator/templates/client_header.py
            ${COMM_PYTHONPATH}/interface_generator/templates/client_source.py
            ${COMM_PYTHONPATH}/interface_generator/templates/server_header.py
            ${COMM_PYTHONPATH}/interface_generator/templates/server_source.py
            ${COMM_PYTHONPATH}/interface_generator/tokens.py
        VERBATIM
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
