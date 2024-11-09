#ifndef LOGR_INCLUDE_LOGR_TRACE
#define LOGR_INCLUDE_LOGR_TRACE

#include <source_location>

#include "internal.hpp"
#include "ILogger.hpp"


namespace LOGR
{

///
/// \brief Log function execution flow and other useful info.
///
/// A `Trace` object automatically logs its creation and destruction,
/// usually used to mark the start and end of a function.
///
/// Use the `log()` function for intermediary information.
///
template <typename... Ts>
class Trace
{
public:
    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) > 0)
        : m_loc(loc)
    {
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "v";
        ((line << " " << std::forward<Ts>(args)), ...);
        line << "\n";
        ILogger::instance()->queueLogLine(line.str());
    }

    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) == 0)
        : m_loc(loc)
    {
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "v\n";
        ILogger::instance()->queueLogLine(line.str());
    }

    ~Trace()
    {
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "^\n";
        ILogger::instance()->queueLogLine(line.str());
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "|";
        ((line << " " << std::forward<T1s>(args)), ...);
        line << "\n";
        ILogger::instance()->queueLogLine(line.str());
    }

private:
    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_TRACE
