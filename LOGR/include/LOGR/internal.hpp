#ifndef LOGR_INCLUDE_LOGR_INTERNAL
#define LOGR_INCLUDE_LOGR_INTERNAL

#include <fstream>
#include <source_location>
#include <sstream>


namespace LOGR
{

namespace internal
{
constexpr char SEPARATOR = ';';

extern thread_local std::ofstream logfile;

enum class Level
{
    TRACE,
    WARNING,
    EXCEPTION
};

std::ostringstream startLine(Level level, const std::source_location& loc);

}

}

#endif // LOGR_INCLUDE_LOGR_INTERNAL
