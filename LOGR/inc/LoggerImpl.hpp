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

///
/// \brief The production ILogger implementation.
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
