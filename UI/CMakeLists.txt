project(UI)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/UserInterface.hpp

    ${INT_INC_PATH}/MoveInput.hpp

    ${SRC_PATH}/UserInterface.cpp
    ${SRC_PATH}/MoveInput.cpp
)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads SYNC DOMN LOGR Driver_interface)

add_subdirectory(test)
