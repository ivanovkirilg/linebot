#include "LOGR/ILogger.hpp"
#include "LOGR/Exception.hpp"
#include "LOGR/Trace.hpp"
#include "LOGR/Warning.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <thread>
#include <string>

#include <ctime>
#include <cstdio>


using namespace LOGR;
using namespace internal;
using src_loc = std::source_location;


namespace
{

struct LogLine
{
    Level level;
    double timestamp;
    std::string threadId;
    std::string file;
    long line;
    std::string function;
    std::string message;
};

LogLine parse(const std::string& line)
{
    LogLine fields;

    int level{};
    double timestamp{};
    char threadId[1024]{};
    char file[1024]{};
    long lineNr{};
    char function[1024]{};
    char message[1024]{};

    // 0;1724138058.333385;140379180749760;/linebot/MAIN/src/main.cpp;16;int main();v
    sscanf(line.c_str(), " %d;%lf;%[^;];%[^;];%li;%[^;];%[^;] ",
           &level, &timestamp, threadId, file, &lineNr, function, message);

    return LogLine {
        static_cast<Level>(level),
        timestamp, threadId, file, lineNr, function, message
    };
}

} // anonymous namespace

TEST(TestLoggerFunctional, CreatesLogfile)
{
    const time_t creationTime = ::time(nullptr);

    ILogger::create("TEST");

    const std::string expectedFile = "LOGR_TEST_"
                                     + std::to_string(creationTime) + ".csv";
    ASSERT_TRUE(std::filesystem::exists(expectedFile));
}

TEST(TestLoggerFunctional, LogsDifferentLevels)
{
    long expectedTraceLocation{};
    long expectedWarnLocation{};
    long expectedExcLocation{};
    long expectedHandleLocation{};

    const time_t creationTime = ::time(nullptr);
    {
        auto logger = ILogger::create("TEST");

        Trace trace("trace");
        expectedTraceLocation = src_loc::current().line() - 1;

        Warning warn("warning");
        expectedWarnLocation = src_loc::current().line() - 1;

        Exception exc("exception");
        expectedExcLocation = src_loc::current().line() - 1;

        exc.handle("handle");
        expectedHandleLocation = src_loc::current().line() - 1;
    }

    const std::string expectedFile = "LOGR_TEST_"
                                     + std::to_string(creationTime) + ".csv";
    std::ifstream file(expectedFile);

    std::ostringstream threadId;
    threadId << std::this_thread::get_id();

    std::string line;
    LogLine log;

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     Level::TRACE);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedTraceLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "v trace");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     Level::WARNING);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedWarnLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "! warning");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     Level::EXCEPTION);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedExcLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "> [1] exception");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     Level::EXCEPTION);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedHandleLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "< [1] handle");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     Level::TRACE);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedTraceLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "^");
}
