project(LOGR_test)

add_executable(
    FT_Logger
    FT_Logger.cpp
)
target_link_libraries(
    FT_Logger
    GTest::gmock_main
    LOGR
)

add_test(NAME LOGR::FT_Logger COMMAND FT_Logger)
