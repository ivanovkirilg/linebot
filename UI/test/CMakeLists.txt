project(UI_test)

add_executable(
    UT_MoveInput
    UT_MoveInput.cpp
)
target_link_libraries(
    UT_MoveInput
    GTest::gmock_main
    UI
    Driver_interface
    DOMN
    LOGR
)

add_test(NAME UI::UT_MoveInput COMMAND UT_MoveInput)
