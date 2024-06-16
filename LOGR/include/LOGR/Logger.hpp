#ifndef LOGR_INCLUDE_LOGR_LOGGER
#define LOGR_INCLUDE_LOGR_LOGGER

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <source_location>
#include <sstream>

namespace LOGR
{

constexpr char SEPARATOR = ';';

namespace internal
{
extern thread_local std::ofstream logfile;
}

template <typename... Ts>
class Trace
{
public:
    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) > 0)
        : m_loc(loc)
    {
        logLinePrefix();
        internal::logfile << SEPARATOR << "v ";
        ((internal::logfile << std::forward<Ts>(args) << " "), ...);
        internal::logfile << "\n";
    }

    Trace(Ts&&... args,
          const std::source_location& loc = std::source_location::current())
          requires(sizeof...(Ts) == 0)
        : m_loc(loc)
    {
        logLinePrefix();
        internal::logfile << SEPARATOR << "v\n";
    }

    ~Trace()
    {
        logLinePrefix();
        internal::logfile << SEPARATOR << "^\n";
    }

    template <typename... T1s>
    void log(T1s&&... args)
    {
        logLinePrefix();
        internal::logfile << SEPARATOR << "| ";
        ((internal::logfile << std::forward<T1s>(args) << " "), ...);
        internal::logfile << "\n";
    }

private:
    void logLinePrefix()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto nowMs = duration_cast<milliseconds>(now.time_since_epoch())
                     % milliseconds(1s);

        std::time_t nowC = system_clock::to_time_t(now);

        std::ostringstream nowStr;

        nowStr << std::put_time(std::localtime(&nowC), "%F %T")
               << '.' << std::setfill('0') << std::setw(3) << nowMs.count();

        internal::logfile << nowStr.str() << SEPARATOR << m_loc.function_name();
    }

    std::source_location m_loc;
};

template <typename... Ts>
Trace(Ts&&...) -> Trace<Ts...>;

}

#endif // LOGR_INCLUDE_LOGR_LOGGER
