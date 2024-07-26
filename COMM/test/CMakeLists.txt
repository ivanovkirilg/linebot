cmake_minimum_required(VERSION 3.14)

project(COMM_test)

add_executable(
    server
    server.cpp
)
target_link_libraries(server PRIVATE COMM)

add_executable(
    client
    client.cpp
)
target_link_libraries(client PRIVATE COMM)

find_package(Python COMPONENTS Interpreter REQUIRED)

add_test(NAME serverConnectionTest
         COMMAND ${Python_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/test_server_connection.py
                 ${CMAKE_CURRENT_BINARY_DIR}/server)

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
