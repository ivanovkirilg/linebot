#ifndef LOGR_INCLUDE_LOGR_LOGGER
#define LOGR_INCLUDE_LOGR_LOGGER

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
    virtual ~LoggerImpl() override;

    virtual void queueLog(internal::Kind level,
        const std::source_location& loc,
        std::string&& message,
        std::thread::id threadId) override;

private:
    void logSome(size_t nrLinesToLog);

    const std::string m_taskName;

    std::queue<internal::Log> m_toLog;
    std::mutex m_toLogMutex;

    std::ofstream m_logFile;

    std::thread m_ioThread;
    std::atomic<bool> m_keepLogging;

    static LoggerImpl *m_logger;
};

}

#endif // LOGR_INCLUDE_LOGR_LOGGER
