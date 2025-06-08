project(CTRL)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

if(Doxygen_FOUND)
    doxygen_add_docs(CTRL_docs)
endif()

add_library(
    ${PROJECT_NAME} SHARED
    ${EXT_INC_PATH}/${PROJECT_NAME}/Controller.hpp

    ${SRC_PATH}/Controller.cpp
)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${EXT_INC_PATH}
)

target_link_libraries(${PROJECT_NAME} PUBLIC DOMN LOGR Driver_interface)

add_subdirectory(test)
