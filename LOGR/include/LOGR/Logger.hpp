#ifndef LOGR_INCLUDE_LOGR_LOGGER
#define LOGR_INCLUDE_LOGR_LOGGER

#include <iostream>
#include <source_location>

namespace LOGR
{

template <typename... Ts>
class Trace
{
public:
    Trace(Ts&&... args, const std::source_location& loc = std::source_location::current())
        : m_loc(loc)
    {
        std::clog << loc.function_name() << " v ";
        ((std::clog << std::forward<Ts>(args) << " "), ...);
        std::clog << "\n";
    }

    ~Trace()
    {
        std::clog << m_loc.function_name() << " ^\n";
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        std::clog << m_loc.function_name() << " | ";
        ((std::clog << std::forward<T1s>(args) << " "), ...);
        std::clog << "\n";
    }

private:
    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_LOGGER
