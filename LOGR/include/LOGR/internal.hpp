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

enum class Kind
{
    TRACE_BEGIN,
    TRACE_LOG,
    TRACE_END,

    WARNING,

    EXCEPTION_RAISE,
    EXCEPTION_HANDLE,
};

struct Log
{
    Kind kind;
    std::chrono::system_clock::duration timestamp;
    std::thread::id threadId;
    std::source_location location;
    std::string message;

    Log(Kind level, const std::source_location& loc, std::string&& message, std::thread::id threadId);

    std::string intoFormatted();
};

}

}

#endif // LOGR_INCLUDE_LOGR_INTERNAL
