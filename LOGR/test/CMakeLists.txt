project(LOGR_test)

add_executable(
    ${PROJECT_NAME}
    FT_Logger.cpp
)
target_link_libraries(
    ${PROJECT_NAME}
    GTest::gmock_main
    LOGR
)

include(GoogleTest)
gtest_discover_tests(LOGR_test)
