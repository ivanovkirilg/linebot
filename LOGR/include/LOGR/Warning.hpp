#ifndef LOGR_INCLUDE_LOGR_WARNING
#define LOGR_INCLUDE_LOGR_WARNING

#include <source_location>

#include "LOGR/internal.hpp"
#include "ILogger.hpp"


namespace LOGR
{

///
/// \brief Log a potentially problematic situation.
///
/// A `Warning` object immediately logs the message (marked as a warning)
/// and is useless afterwards - it is meant to be created as a temporary.
///
/// This is still a class to keep a consistent interface with other logging
/// classes; and for an easier template interface.
///
template <typename... Ts>
class Warning
{
public:
    Warning(Ts&&... args,
        const std::source_location& loc = std::source_location::current())
    {
        std::ostringstream line;
        ((line << " " << std::forward<Ts>(args)), ...);

        ILogger::instance()->queueLog(internal::Level::WARNING, loc, std::move(line).str());
    }
};

template <typename... Ts>
Warning(Ts&&...) -> Warning<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_WARNING
