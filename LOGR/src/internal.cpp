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
    switch (level)
    {
        case Level::TRACE:     levelStr = "TRACE"; break;
        case Level::WARNING:   levelStr = "WARN";  break;
        case Level::EXCEPTION: levelStr = "EXC";   break;
    }

    std::ostringstream line;
    line << std::move(timeStr).str() << SEPARATOR
         << levelStr                 << SEPARATOR
         << threadId                 << SEPARATOR
         << location.file_name()     << SEPARATOR
         << location.line()          << SEPARATOR
         << location.function_name() << SEPARATOR
         << message // NOTE: messages are not sanitized!!!
        ;
    return std::move(line).str();
}
