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

## FT_Watcher
add_executable(
    FT_Watcher_SUT
    FT_Watcher/SUT.cpp
)
target_link_libraries(FT_Watcher_SUT PRIVATE COMM)
add_test(
    NAME COMM::FT_WatcherTest
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
    NAME COMM::FT_ServerTest
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/FT_Server/test_server.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_Server_SUT
)

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
