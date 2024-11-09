#ifndef LOGR_INCLUDE_LOGR_INTERNAL
#define LOGR_INCLUDE_LOGR_INTERNAL

#include <source_location>
#include <sstream>


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

std::ostringstream startLine(Level level, const std::source_location& loc);

}

}

#endif // LOGR_INCLUDE_LOGR_INTERNAL
