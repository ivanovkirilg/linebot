project(UI_test)

add_executable(
    UI_test
    UT_MoveInput.cpp
)
target_link_libraries(
    UI_test
    GTest::gmock_main
    UI
    DRVR
    DOMN
    LOGR
)

include(GoogleTest)
gtest_discover_tests(UI_test)
