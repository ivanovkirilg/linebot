project(COMM_XMPL)

# set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
# set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
# set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# include_directories(${INT_INC_PATH})

add_executable(
    ${PROJECT_NAME}_server
    server_main.cpp
    Driver.cpp
    DriverServer.cpp
)

add_executable(
    ${PROJECT_NAME}_client
    client_main.cpp
    DriverClient.cpp
)

# target_include_directories(
#     ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
# )

target_link_libraries(${PROJECT_NAME}_server PRIVATE LOGR COMM)
target_link_libraries(${PROJECT_NAME}_client PRIVATE COMM)
