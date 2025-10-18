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

add_executable(
    FT_LoggerPerf
    FT_LoggerPerf.cpp
)
target_link_libraries(
    FT_LoggerPerf
    LOGR
)

add_custom_target(
    LOGR_perf_baseline
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_SOURCE_DIR}/COMM/test" # FIXME
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/test_logger_perf.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_LoggerPerf
        "--baseline"
    DEPENDS FT_LoggerPerf
)

add_custom_target(
    LOGR_perf_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_SOURCE_DIR}/COMM/test" # FIXME
        ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/test_logger_perf.py
        ${CMAKE_CURRENT_BINARY_DIR}/FT_LoggerPerf
        "--test"
    DEPENDS FT_LoggerPerf
)
