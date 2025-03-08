cmake_minimum_required(VERSION 3.14)

project(COMM_test)

## FT_Connection
add_executable(
    FT_Connection_SUT
    FT_Connection/SUT.cpp
)
target_link_libraries(FT_Connection_SUT PRIVATE COMM)
add_test(
    NAME COMM::FT_ConnectionTest
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
    NAME COMM::FT_SocketTest
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_Socket/test_socket.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_Socket_SUT
)


## Interface

add_executable(
    interface_server
    test_interface/interface_server.cpp
)
target_link_libraries(interface_server PRIVATE COMM)

add_test(NAME serverInterfaceTest
         COMMAND ${Python_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/test_interface/test_interface_server.py
                 ${CMAKE_CURRENT_BINARY_DIR}/interface_server)


# add_executable(
#     TODO_test
#     test_reader.cpp
# )
# target_link_libraries(
#     TODO_test
#     GTest::gtest_main
#     ${PROJECT_NAME}_lib
# )

include(GoogleTest)
# gtest_discover_tests(TODO_test)
