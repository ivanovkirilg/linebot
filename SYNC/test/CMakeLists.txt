project(SYNC_test)

add_executable(
    FT_Tick
    FT_Tick.cpp
    ${SRC_PATH}/Tick.cpp
)
target_link_libraries(FT_Tick GTest::gtest_main LOGR pthread)
enable_target_warnings(FT_Tick)

add_test(NAME SYNC::FT_Tick COMMAND FT_Tick)
