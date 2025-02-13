cmake_minimum_required(VERSION 3.14)

project(DRVR_test)

add_executable(
    FT_Driver
    FT_Driver.cpp
    ${SRC_PATH}/Driver.cpp
)
target_link_libraries(
    FT_Driver
    Driver_interface
    DOMN
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FT_Driver)
