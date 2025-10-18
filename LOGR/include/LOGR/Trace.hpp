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
        std::ostringstream line;
        ((line << " " << std::forward<Ts>(args)), ...);
        ILogger::instance()->queueLog(internal::Kind::TRACE_BEGIN, m_loc, std::move(line).str());
    }

    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) == 0)
        : m_loc(loc)
    {
        (args, ...); // Silence 'Wunused' for compiler versions that don't understand requires()
        ILogger::instance()->queueLog(internal::Kind::TRACE_BEGIN, m_loc, "");
    }

    ~Trace()
    {
        ILogger::instance()->queueLog(internal::Kind::TRACE_END, m_loc, "");
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        std::ostringstream line;
        ((line << " " << std::forward<T1s>(args)), ...);
        ILogger::instance()->queueLog(internal::Kind::TRACE_LOG, m_loc, std::move(line).str());
    }

private:
    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_TRACE
