cmake_minimum_required(VERSION 3.14)

project(DRVR)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(INTERFACE_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${INT_INC_PATH})

# add_library(
#     ${PROJECT_NAME} SHARED
#     ${EXT_INC_PATH}/${PROJECT_NAME}/Driver.hpp

#     ${SRC_PATH}/Driver.cpp
# )
# enable_target_warnings(${PROJECT_NAME})

# target_include_directories(
#     ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
# )

# target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)


add_interface(DRVR Driver)

add_executable(
    DRVR_main
        ${INT_INC_PATH}/Driver.hpp

        ${SRC_PATH}/Driver_main.cpp
        ${SRC_PATH}/Driver.cpp
)

target_link_libraries(DRVR_main PRIVATE Driver_interface)

add_subdirectory(test)
