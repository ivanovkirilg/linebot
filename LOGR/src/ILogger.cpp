#include "LOGR/ILogger.hpp"

#include <iostream>

#include "LoggerImpl.hpp"
#include "LOGR/internal.hpp"

using namespace LOGR::internal;

namespace LOGR
{

std::weak_ptr<ILogger> ILogger::m_instance = {};

std::shared_ptr<ILogger> ILogger::create(const std::string& taskName)
{
    if (m_instance.use_count() > 0)
    {
        throw LoggerAlreadySet(
            "Only one Logger per executable can exist at a time.");
    }

    auto logger = std::make_shared<LoggerImpl>(taskName);
    m_instance = logger;
    return logger;
}

std::shared_ptr<ILogger> ILogger::instance()
{
    auto instance = m_instance.lock();
    if (not instance)
    {
        throw LoggerNotSet(
            "A Logger object must be created before logging.");
    }
    return instance;
}


class LoggerStub : public ILogger
{
public:
    LoggerStub(const std::string& taskName)
        : m_taskName(taskName)
    { }

protected:
    virtual void queueLog(Kind level, const std::source_location& loc,
            std::string&& message, std::thread::id threadId) override
    {
        Log log{level, loc, std::move(message), threadId};

        std::unique_lock logLock(m_logMutex);
        std::cerr << "[" << m_taskName << "]" << SEPARATOR
                  << log.intoFormatted();
    }

private:
    std::mutex m_logMutex;
    const std::string m_taskName;
};

std::shared_ptr<ILogger> ILogger::create(const std::string& taskName, StubSelection stub)
{
    (void) stub;
    auto logger = std::make_shared<LoggerStub>(taskName);
    m_instance = std::dynamic_pointer_cast<ILogger>(logger);
    return std::dynamic_pointer_cast<ILogger>(logger);
}

}
