#ifndef LOGR_INCLUDE_LOGR_LOGGER
#define LOGR_INCLUDE_LOGR_LOGGER

#include <atomic>
#include <fstream>
#include <source_location>
#include <stdexcept>


namespace LOGR
{

constexpr char SEPARATOR = ';';

namespace internal
{
extern thread_local std::ofstream logfile;

enum class Severity
{
    TRACE,
    WARNING,
    EXCEPTION
};

void logLinePrefix(Severity severity, const std::source_location& loc);

}

class Exception : public std::runtime_error
{
public:
    Exception(const std::string& message,
        const std::source_location& loc = std::source_location::current());

    void handle(const std::string& message,
        const std::source_location& loc = std::source_location::current()) const;

private:
    static std::atomic<unsigned long long> freeId;
    unsigned long long id{};
};

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
        internal::logfile << "v ";
        ((internal::logfile << std::forward<Ts>(args) << " "), ...);
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
        internal::logfile << "| ";
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
