#include "LOGR/ILogger.hpp"

#include <iostream>

#include "LOGR/Logger.hpp"
#include "LOGR/internal.hpp"


namespace LOGR
{

std::weak_ptr<ILogger> ILogger::m_instance = {};

class LoggerStub : public ILogger
{
public:
    LoggerStub(const std::string& taskName)
        : m_taskName(taskName)
    { }

protected:
    virtual void queueLogLine(const std::string& line) override
    {
        std::unique_lock logLock(m_logMutex);
        std::cerr << "[" << m_taskName << "]" << internal::SEPARATOR << line;
    }

private:
    std::mutex m_logMutex;
    const std::string m_taskName;
};

std::shared_ptr<ILogger> ILogger::create(const std::string& taskName)
{
    if (m_instance.use_count() > 0)
    {
        throw LoggerAlreadySet("You must create exactly one Logger object "
                               "per executable (or at least at one time).");
    }

    auto logger = std::make_shared<Logger>(taskName);
    m_instance = logger;
    return logger;
}

std::shared_ptr<ILogger> ILogger::instance()
{
    auto instance = m_instance.lock();
    if (not instance)
    {
        throw LoggerNotSet("You must create exactly one Logger object "
                           "per executable (or at least at one time).");
    }
    return instance;
}

std::shared_ptr<ILogger> ILoggerStub::create(const std::string& taskName)
{
    auto logger = std::make_shared<LoggerStub>(taskName);
    m_instance = logger;
    return logger;
}


}
