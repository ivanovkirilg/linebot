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

add_executable(
    FT_LoggerPerf
    FT_LoggerPerf.cpp
)
target_link_libraries(
    FT_LoggerPerf
    LOGR
)

include(GoogleTest)
gtest_discover_tests(LOGR_test)
