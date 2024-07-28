#ifndef LOGR_INCLUDE_LOGR_TRACE
#define LOGR_INCLUDE_LOGR_TRACE

#include <source_location>

#include "internal.hpp"
#include "Logger.hpp"


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
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "v";
        ((line << " " << std::forward<Ts>(args)), ...);
        line << "\n";
        Logger::queueLogLine(line.str());
    }

    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) == 0)
        : m_loc(loc)
    {
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "v\n";
        Logger::queueLogLine(line.str());
    }

    ~Trace()
    {
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "^\n";
        Logger::queueLogLine(line.str());
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        auto line = internal::startLine(internal::Level::TRACE, m_loc);
        line << "|";
        ((line << " " << std::forward<T1s>(args)), ...);
        line << "\n";
        Logger::queueLogLine(line.str());
    }

private:
    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_TRACE
