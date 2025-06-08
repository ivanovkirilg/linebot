project(DRVR_test)

add_executable(
    FT_Driver
    FT_Driver.cpp
    ${SRC_PATH}/Driver.cpp
)
target_link_libraries(
    FT_Driver
    Driver_interface
    GTest::gtest_main
)
enable_target_warnings(FT_Driver)

include(GoogleTest)
gtest_discover_tests(FT_Driver)
