#ifndef LOGR_INCLUDE_LOGR_LOGGER
#define LOGR_INCLUDE_LOGR_LOGGER

#include <stdexcept>
#include <string>
#include <fstream>

#include <atomic>
#include <mutex>
#include <thread>
#include <queue>

#include "LOGR/ILogger.hpp"


namespace LOGR
{

class LoggerAlreadySet : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class LoggerNotSet : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

///
/// \brief Manage a log-file per 'task' / process
///
/// You are expected to create a single `Logger` object in your `main()`
/// function, which enables the use of the other logging classes.
///
/// All logging is asynchronous and safe to use from multiple threads.
///
class LoggerImpl : public ILogger
{
public:
    LoggerImpl(const std::string& taskName);
    ~LoggerImpl();

    virtual void queueLogLine(const std::string& line) override;

private:
    void logSome(size_t nrLinesToLog);

    const std::string m_taskName;

    std::queue<std::string> m_toLog;
    std::mutex m_toLogMutex;

    std::ofstream m_logFile;

    std::thread m_ioThread;
    std::atomic<bool> m_keepLogging;

    static LoggerImpl *m_logger;
};

}

#endif // LOGR_INCLUDE_LOGR_LOGGER
