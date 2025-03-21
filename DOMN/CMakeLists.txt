project(DOMN)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INT_INC_PATH})

add_library(
    ${PROJECT_NAME} INTERFACE
    ${EXT_INC_PATH}/${PROJECT_NAME}/Move.hpp
)

target_include_directories(${PROJECT_NAME} INTERFACE ${EXT_INC_PATH})

add_subdirectory(test)
