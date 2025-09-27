#include "LOGR/ILogger.hpp"
#include "LOGR/Exception.hpp"
#include "LOGR/Trace.hpp"
#include "LOGR/Warning.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include <filesystem>
#include <fstream>
#include <thread>
#include <string>

#include <ctime>
#include <cstdio>


using namespace LOGR;
using namespace internal;
using src_loc = std::source_location;

using testing::StartsWith;
using testing::HasSubstr;


namespace
{

struct LogLine
{
    double timestamp;
    std::string level;
    std::string threadId;
    std::string file;
    long line;
    std::string function;
    std::string message;
};

LogLine parse(const std::string& line)
{
    LogLine fields;

    char level[16]{};
    double timestamp{};
    char threadId[1024]{};
    char file[1024]{};
    long lineNr{};
    char function[1024]{};
    char message[1024]{};

    // 0;1724138058.333385;140379180749760;/linebot/MAIN/src/main.cpp;16;int main();v
    sscanf(line.c_str(), " %lf;%15[^;];%1023[^;];%1023[^;];%li;%1023[^;];%1023[^;] ",
           &timestamp, level, threadId, file, &lineNr, function, message);

    return LogLine {
        timestamp, level, threadId, file, lineNr, function, message
    };
}

constexpr const char* TRACE = "TRACE";
constexpr const char* WARNING = "WARN";
constexpr const char* EXCEPTION = "EXC";

} // anonymous namespace

class TestLoggerFunctional : public ::testing::Test
{
protected:
    void TearDown() override
    {
        std::filesystem::remove(expectedFile);
    }

    const time_t creationTime = ::time(nullptr);
    const std::string expectedFile = "LOGR_TEST_"
                                     + std::to_string(creationTime)
                                     + ".csv";
};

TEST_F(TestLoggerFunctional, CreatesLogfile)
{
    ILogger::create("TEST");

    ASSERT_TRUE(std::filesystem::exists(expectedFile));
}

TEST_F(TestLoggerFunctional, LogsDifferentLevels)
{
    long expectedTraceLocation{};
    long expectedWarnLocation{};
    long expectedExcLocation{};
    long expectedHandleLocation{};

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

    std::ifstream file(expectedFile);

    std::ostringstream threadId;
    threadId << std::this_thread::get_id();

    std::string line;
    LogLine log;

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     TRACE);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedTraceLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "v trace");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     WARNING);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedWarnLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "! warning");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     EXCEPTION);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedExcLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "> [1] exception");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     EXCEPTION);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedHandleLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "< [1] handle");

    ASSERT_TRUE(std::getline(file, line));
    log = parse(line);

    EXPECT_EQ(  log.level,     TRACE);
    EXPECT_NEAR(log.timestamp, creationTime, 1);
    EXPECT_EQ(  log.threadId,  threadId.str());
    EXPECT_EQ(  log.file,      src_loc::current().file_name());
    EXPECT_EQ(  log.line,      expectedTraceLocation);
    EXPECT_EQ(  log.function,  src_loc::current().function_name());
    EXPECT_EQ(  log.message,   "^");
}

TEST(TestGetUnderlyingError, ReturnsMessage)
{
    std::ifstream nonExistentFile;
    nonExistentFile.open("justsomenonexistentfileIhopenoonecreatesit.txt");

    ASSERT_FALSE(nonExistentFile.is_open());

    const std::string prefix = std::to_string(ENOENT) + ": ";

    EXPECT_THAT(LOGR::getUnderlyingError(), StartsWith(prefix));
    EXPECT_THAT(LOGR::getUnderlyingError(), HasSubstr("No such file"));
}
