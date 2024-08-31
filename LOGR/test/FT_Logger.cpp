#include "LOGR/ILogger.hpp"

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
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
    const time_t creationTime = ::time(nullptr);

    LOGR::ILogger::create("TEST");

    const std::string expectedFile = "LOGR_TEST_"
                                     + std::to_string(creationTime) + ".csv";
    ASSERT_TRUE(std::filesystem::exists(expectedFile));
}

TEST(TestLoggerFunctional, LogsSimpleLines)
{
    const std::array<std::string, 2> lines = { "hello", "world" };

    const time_t creationTime = ::time(nullptr);

    {
        auto logger = LOGR::ILogger::create("TEST");

        for (const auto& line : lines)
        {
            LOGR::Exception::queueLogLine(line + '\n');
        }
    }

    const std::string expectedFile = "LOGR_TEST_"
                                     + std::to_string(creationTime) + ".csv";
    std::ifstream file(expectedFile);

    for (const auto& line : lines)
    {
        std::string actual;

        ASSERT_TRUE(std::getline(file, actual));
        EXPECT_EQ(line, actual);
    }
}
