project(UI)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/UserInterface.hpp

    ${SRC_PATH}/UserInterface.cpp
)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE DOMN LOGR DRVR)

add_subdirectory(test)
