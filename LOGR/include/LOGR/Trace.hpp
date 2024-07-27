#ifndef LOGR_INCLUDE_LOGR_TRACE
#define LOGR_INCLUDE_LOGR_TRACE

#include <source_location>

#include "internal.hpp"


namespace LOGR
{

template <typename... Ts>
class Trace
{
public:
    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) > 0)
        : m_loc(loc)
    {
        internal::logLinePrefix(internal::Severity::TRACE, m_loc);
        internal::logfile << "v";
        ((internal::logfile << " " << std::forward<Ts>(args)), ...);
        internal::logfile << "\n";
    }

    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) == 0)
        : m_loc(loc)
    {
        internal::logLinePrefix(internal::Severity::TRACE, m_loc);
        internal::logfile << "v\n";
    }

    ~Trace()
    {
        internal::logLinePrefix(internal::Severity::TRACE, m_loc);
        internal::logfile << "^\n";
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        internal::logLinePrefix(internal::Severity::TRACE, m_loc);
        internal::logfile << "|";
        ((internal::logfile << " " << std::forward<T1s>(args)), ...);
        internal::logfile << "\n";
    }

private:
    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_TRACE
