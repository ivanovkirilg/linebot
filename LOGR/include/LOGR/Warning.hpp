#ifndef LOGR_INCLUDE_LOGR_WARNING
#define LOGR_INCLUDE_LOGR_WARNING

#include <source_location>

#include "LOGR/internal.hpp"
#include "LOGR/Logger.hpp"


namespace LOGR
{

template <typename... Ts>
class Warning
{
public:
    Warning(Ts&&... args,
        const std::source_location& loc = std::source_location::current())
    {
        auto line = internal::startLine(internal::Severity::WARNING, loc);
        line << "!";
        ((line << " " << std::forward<Ts>(args)), ...);
        line << "\n";
        Logger::queueLogLine(line.str());
    }
};

template <typename... Ts>
Warning(Ts&&...) -> Warning<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_WARNING
