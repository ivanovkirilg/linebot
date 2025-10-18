#include "LOGR/internal.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

using namespace LOGR;
using namespace LOGR::internal;

namespace
{

constexpr const char* LEVEL_UNKNOWN   = "???  ";
constexpr const char* LEVEL_TRACE     = "TRACE";
constexpr const char* LEVEL_WARNING   = "WARN ";
constexpr const char* LEVEL_EXCEPTION = "EXC  ";


constexpr char PREFIX_UNKNOWN = '?';

constexpr char PREFIX_TRACE_BEGIN = 'v';
constexpr char PREFIX_TRACE_LOG   = '|';
constexpr char PREFIX_TRACE_END   = '^';

constexpr char PREFIX_WARNING = '!';

constexpr char PREFIX_EXCEPTION_RAISE  = '>';
constexpr char PREFIX_EXCEPTION_HANDLE = '<';

} // namespace

Log::Log(Kind lvl, const std::source_location& loc,
        std::string&& msg, std::thread::id thrd)
    :
        kind(lvl),
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

    const char* levelStr = LEVEL_UNKNOWN;
    char prefix = PREFIX_UNKNOWN;

    switch (kind)
    {
        case Kind::TRACE_BEGIN:
        case Kind::TRACE_LOG:
        case Kind::TRACE_END:
        {
            levelStr = LEVEL_TRACE;
        } break;

        case Kind::WARNING:
        {
            levelStr = LEVEL_WARNING;
        } break;

        case Kind::EXCEPTION_RAISE:
        case Kind::EXCEPTION_HANDLE:
        {
            levelStr = LEVEL_EXCEPTION;
        } break;
    }

    switch (kind)
    {
        case Kind::TRACE_BEGIN:      prefix = PREFIX_TRACE_BEGIN;      break;
        case Kind::TRACE_LOG:        prefix = PREFIX_TRACE_LOG;        break;
        case Kind::TRACE_END:        prefix = PREFIX_TRACE_END;        break;
        case Kind::WARNING:          prefix = PREFIX_WARNING;          break;
        case Kind::EXCEPTION_RAISE:  prefix = PREFIX_EXCEPTION_RAISE;  break;
        case Kind::EXCEPTION_HANDLE: prefix = PREFIX_EXCEPTION_HANDLE; break;
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
