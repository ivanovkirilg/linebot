cmake_minimum_required(VERSION 3.14)

project(LOGR_test)

add_executable(
    ${PROJECT_NAME}
    test_Logger.cpp
)
target_link_libraries(
    ${PROJECT_NAME}
    GTest::gtest_main
    LOGR
)

include(GoogleTest)
gtest_discover_tests(LOGR_test)
