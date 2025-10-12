#include "LOGR/internal.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

using namespace LOGR;
using namespace LOGR::internal;

Log::Log(Level lvl, const std::source_location& loc,
        std::string&& msg, std::thread::id thrd)
    :
        level(lvl),
        timestamp(std::chrono::system_clock::now().time_since_epoch()),
        threadId(thrd),
        location(loc),
        message(msg)
{
}

std::string Log::intoFormatted()
{
    using namespace std::chrono;
    const auto seconds = duration_cast<std::chrono::seconds>(timestamp);
    const auto microseconds = duration_cast<std::chrono::microseconds>(timestamp) % 1s;

    std::ostringstream timeStr;

    timeStr << seconds.count()
            << '.'
            << std::setfill('0') << std::setw(6)
            << microseconds.count();

    const char* levelStr = "???";
    const char* prefix = "?";
    switch (level)
    {
        case Level::TRACE_BEGIN:
        {
            levelStr = "TRACE";
            prefix = "v";
        } break;

        case Level::TRACE_LOG:
        {
            levelStr = "TRACE";
            prefix = "|";
        } break;

        case Level::TRACE_END:
        {
            levelStr = "TRACE";
            prefix = "^";
        } break;

        case Level::WARNING:
        {
            levelStr = "WARN";
            prefix = "!";
        } break;

        case Level::EXCEPTION_RAISE:
        {
            levelStr = "EXC";
            prefix = ">";
        } break;

        case Level::EXCEPTION_HANDLE:
        {
            levelStr = "EXC";
            prefix = "<";
        } break;
    }

    std::ostringstream line;
    line << std::move(timeStr).str() << SEPARATOR
         << levelStr                 << SEPARATOR
         << threadId                 << SEPARATOR
         << location.file_name()     << SEPARATOR
         << location.line()          << SEPARATOR
         << location.function_name() << SEPARATOR
         << prefix << message << '\n'; // NOTE: messages are not sanitized!!!

    return std::move(line).str();
}
