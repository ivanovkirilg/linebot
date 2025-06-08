project(DRVR)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INT_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(INTERFACE_PATH ${CMAKE_CURRENT_BINARY_DIR})

if(Doxygen_FOUND)
    doxygen_add_docs(DRVR_docs)
endif()

include_directories(${INT_INC_PATH})

add_interface(DRVR Driver)

add_executable(
    DRVR_main
    ${INT_INC_PATH}/Driver.hpp

    ${SRC_PATH}/Driver_main.cpp
    ${SRC_PATH}/Driver.cpp
)
enable_target_warnings(DRVR_main)

target_link_libraries(DRVR_main Threads::Threads Driver_interface LOGR)

add_subdirectory(test)
