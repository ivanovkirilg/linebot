#ifndef LOGR_INCLUDE_LOGR_ILOGGER
#define LOGR_INCLUDE_LOGR_ILOGGER

#include <memory>
#include <string>

namespace LOGR
{

class LoggerNotSet : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class LoggerAlreadySet : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

///
/// \brief Manage a log-file per 'task' / process.
///
/// You are expected to `create()` a single `Logger` object in your `main()`
/// function, which enables the use of the other logging classes for this
/// object's lifetime. The log-file is closed upon destruction.
///
/// All logging is asynchronous and safe to use from multiple threads.
///
class ILogger
{
public:
    static std::shared_ptr<ILogger> create(const std::string& taskName);
    virtual ~ILogger() = default;

protected:
    template<typename... Args>
    friend class Trace;
    template <typename... Ts>
    friend class Warning;
    friend class Exception;

    static std::shared_ptr<ILogger> instance();
    virtual void queueLogLine(const std::string& line) = 0;

    static std::weak_ptr<ILogger> m_instance;
};

///
/// \brief Stub for unit-tests, logs to stderr.
///
/// Use this `create()` instead of the production one to enable your system
/// under test to use the other logging classes.
///
/// All logging calls are thread-safe, but blocking unlike in production.
///
class ILoggerStub : public ILogger
{
public:
    static std::shared_ptr<ILogger> create(const std::string& taskName);
};

}

#endif // LOGR_INCLUDE_LOGR_ILOGGER
