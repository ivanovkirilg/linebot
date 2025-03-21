project(CTRL_test)

add_executable(
    ${PROJECT_NAME}
    UT_Controller.cpp
)
target_link_libraries(
    ${PROJECT_NAME}
    GTest::gmock_main
    CTRL
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
