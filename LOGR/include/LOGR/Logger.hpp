#ifndef LOGR_INCLUDE_LOGR_LOGGER
#define LOGR_INCLUDE_LOGR_LOGGER

#include <fstream>
#include <source_location>

namespace LOGR
{

namespace internal
{
extern std::ofstream logfile;
}

template <typename... Ts>
class Trace
{
public:
    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
        : m_loc(loc)
    {
        internal::logfile << loc.function_name() << " v ";
        ((internal::logfile << std::forward<Ts>(args) << " "), ...);
        internal::logfile << "\n";
    }

    ~Trace()
    {
        internal::logfile << m_loc.function_name() << " ^\n";
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        internal::logfile << m_loc.function_name() << " | ";
        ((internal::logfile << std::forward<T1s>(args) << " "), ...);
        internal::logfile << "\n";
    }

private:
    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_LOGGER
