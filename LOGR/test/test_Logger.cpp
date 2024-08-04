#include "LOGR/ILogger.hpp"

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <ctime>

namespace LOGR
{
class Exception
{
public:
    static void queueLogLine(const std::string &line)
    {
        ILogger::instance()->queueLogLine(line);
    }
};
}


TEST(TestLoggerFunctional, CreatesLogfile)
{
    auto t = ::time(nullptr);
    std::string expectedFile = "LOGR_TEST_" + std::to_string(t) + ".csv";

    LOGR::ILogger::create("TEST");

    ASSERT_TRUE(std::filesystem::exists(expectedFile));
}

TEST(TestLoggerFunctional, LogsSimpleLines)
{
    auto t = ::time(nullptr);
    std::string expectedFile = "LOGR_TEST_" + std::to_string(t) + ".csv";

    std::array<std::string, 2> lines = { "hello", "world" };

    {
        auto logger = LOGR::ILogger::create("TEST");

        for (const auto& line : lines)
        {
            LOGR::Exception::queueLogLine(line + '\n');
        }
    }

    std::ifstream file(expectedFile);
    std::string line;

    for (const auto& line : lines)
    {
        std::string actual;

        ASSERT_TRUE(std::getline(file, actual));
        EXPECT_EQ(line, actual);
    }
}
