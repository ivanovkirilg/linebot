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
enable_target_warnings(FT_Driver)
target_compile_options(FT_Driver PRIVATE -coverage)
target_link_options(FT_Driver PRIVATE -coverage)

include(GoogleTest)
gtest_discover_tests(FT_Driver)
