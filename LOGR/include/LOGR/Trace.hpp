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
        line << "v";
        ((line << " " << std::forward<Ts>(args)), ...);
        line << "\n";
        ILogger::instance()->queueLog(internal::Level::TRACE, m_loc, std::move(line).str());
    }

    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) == 0)
        : m_loc(loc)
    {
        (args, ...); // Silence 'Wunused' for compiler versions that don't understand requires()
        ILogger::instance()->queueLog(internal::Level::TRACE, m_loc, "v\n");
    }

    ~Trace()
    {
        ILogger::instance()->queueLog(internal::Level::TRACE, m_loc, "^\n");
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        std::ostringstream line;
        line << "|";
        ((line << " " << std::forward<T1s>(args)), ...);
        line << "\n";
        ILogger::instance()->queueLog(internal::Level::TRACE, m_loc, std::move(line).str());
    }

private:
    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_TRACE
