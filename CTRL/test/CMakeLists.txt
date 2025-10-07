project(CTRL_test)

add_executable(
    UT_Controller
    UT_Controller.cpp
)
target_link_libraries(
    UT_Controller
    GTest::gmock_main
    CTRL
)

add_test(NAME CTRL::UT_Controller COMMAND UT_Controller)
