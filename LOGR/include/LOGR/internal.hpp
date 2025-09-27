#ifndef LOGR_INCLUDE_LOGR_INTERNAL
#define LOGR_INCLUDE_LOGR_INTERNAL

#include <chrono>
#include <source_location>
#include <string>
#include <thread>


namespace LOGR
{

namespace internal
{

constexpr char SEPARATOR = ';';

enum class Level
{
    TRACE,
    WARNING,
    EXCEPTION
};

struct Log
{
    Level level;
    std::chrono::system_clock::duration timestamp;
    std::thread::id threadId;
    std::source_location location;
    std::string message;

    Log(Level level, const std::source_location& loc, std::string&& message, std::thread::id threadId);

    std::string intoFormatted();
};

}

}

#endif // LOGR_INCLUDE_LOGR_INTERNAL
