#ifndef LOGR_INCLUDE_LOGR_INTERNAL
#define LOGR_INCLUDE_LOGR_INTERNAL

#include <fstream>
#include <source_location>


namespace LOGR
{

namespace internal
{
constexpr char SEPARATOR = ';';

extern thread_local std::ofstream logfile;

enum class Severity
{
    TRACE,
    WARNING,
    EXCEPTION
};

void logLinePrefix(Severity severity, const std::source_location& loc);

}

}

#endif // LOGR_INCLUDE_LOGR_INTERNAL
