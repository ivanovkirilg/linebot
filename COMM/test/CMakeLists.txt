project(COMM_test)

## FT_Connection
add_executable(
    FT_Connection_SUT
    FT_Connection/SUT.cpp
)
target_link_libraries(FT_Connection_SUT PRIVATE COMM)
add_test(
    NAME COMM::FT_Connection
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_Connection/test_connection.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_Connection_SUT
)

## FT_Socket
add_executable(
    FT_Socket_SUT
    FT_Socket/SUT.cpp
)
target_link_libraries(FT_Socket_SUT PRIVATE COMM)
add_test(
    NAME COMM::FT_Socket
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_Socket/test_socket.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_Socket_SUT
)

## FT_Watcher
add_executable(
    FT_Watcher_SUT
    FT_Watcher/SUT.cpp
)
target_link_libraries(FT_Watcher_SUT PRIVATE COMM)
add_test(
    NAME COMM::FT_Watcher
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_Watcher/test_watcher.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_Watcher_SUT
)

## FT_Server
add_executable(
    FT_Server_SUT
    FT_Server/SUT.cpp
)
target_link_libraries(FT_Server_SUT PRIVATE COMM)
add_test(
    NAME COMM::FT_Server
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_Server/test_server.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_Server_SUT
)

## UT_interface_generator (py tests)
add_test(
    NAME COMM::UT_interface_generator
    WORKING_DIRECTORY ${COMM_PYTHONPATH}
    COMMAND
        ${Python_EXECUTABLE} -m unittest -v
)

## FT_interface_generator (example interface)
set(TEST_EXAMPLE_DIR "${COMM_PYTHONPATH}/interface_generator/test/example")
add_executable(
    FT_interface_generator_server_SUT
        ${TEST_EXAMPLE_DIR}/ExampleServer.hpp
        ${TEST_EXAMPLE_DIR}/ExampleServer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_interface_generator/ExampleImpl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_interface_generator/ExampleImpl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_interface_generator/Example_server_main.cpp
)
target_include_directories(
    FT_interface_generator_server_SUT PRIVATE ${TEST_EXAMPLE_DIR}
)
target_link_libraries(
    FT_interface_generator_server_SUT PRIVATE COMM
)

add_executable(
    FT_interface_generator_client_SUT
        ${TEST_EXAMPLE_DIR}/ExampleClient.hpp
        ${TEST_EXAMPLE_DIR}/ExampleClient.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_interface_generator/Example_client_main.cpp
)
target_include_directories(
    FT_interface_generator_client_SUT PRIVATE ${TEST_EXAMPLE_DIR}
)
target_link_libraries(
    FT_interface_generator_client_SUT PRIVATE COMM
)

add_test(
    NAME COMM::FT_interface_generator
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_interface_generator/test_interface_generator.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_interface_generator_server_SUT
        ${CMAKE_CURRENT_BINARY_DIR}/FT_interface_generator_client_SUT
)

## Manual diagnostics test
add_custom_target(COMM_diag
    ${CMAKE_COMMAND} -E env "PYTHONPATH=${COMM_PYTHONPATH}"
    ${Python_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/test_diagnostics.py -m interface_diagnostics.md
    ${CMAKE_CURRENT_SOURCE_DIR}/Diagnostics/Invalid
    ${CMAKE_CURRENT_SOURCE_DIR}/Diagnostics/MissingParamDirection
    ${CMAKE_CURRENT_SOURCE_DIR}/Diagnostics/MissingParamName
    ${CMAKE_CURRENT_SOURCE_DIR}/Diagnostics/MissingParamType
    ${CMAKE_CURRENT_SOURCE_DIR}/Diagnostics/MisspelledWords
    ${CMAKE_CURRENT_SOURCE_DIR}/Diagnostics/Unterminated
)
