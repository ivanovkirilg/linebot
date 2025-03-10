#include "LoggerImpl.hpp"

#include <chrono>
#include <sstream>
#include <vector>

namespace
{

constexpr int LINES_TO_LOG_AT_ONCE = 20;
constexpr int BASE_DELAY = 500;
constexpr int MIN_DELAY = BASE_DELAY / 10;

constexpr int WEIGHT_PER_LINE_FOR_DELAY = BASE_DELAY / LINES_TO_LOG_AT_ONCE;

}


namespace LOGR
{

LoggerImpl * LoggerImpl::m_logger = nullptr;

static inline std::string generateLogfileName(const std::string& taskName)
{
    using namespace std::chrono;
    auto now = system_clock::now().time_since_epoch();
    auto nowSec  = duration_cast<seconds>(now);

    std::stringstream stream;
    stream << "LOGR_" << taskName << '_' << nowSec.count() << ".csv";
    return stream.str();
}

void LoggerImpl::queueLogLine(const std::string& line)
{
    if (not m_logger)
    {
        throw LoggerNotSet("You must create exactly one Logger object "
                           "per executable (or at least at one time).");
    }

    std::unique_lock lock(m_logger->m_toLogMutex);

    m_logger->m_toLog.emplace(line);
}

void LoggerImpl::logSome(size_t nrLinesToLog)
{
    std::vector<std::string> toLog;
    {
        std::unique_lock lock(m_toLogMutex);

        nrLinesToLog = std::min(m_toLog.size(), nrLinesToLog);
        for (size_t i = 0; i < nrLinesToLog; i++)
        {
            toLog.emplace_back(m_toLog.front());
            m_toLog.pop();
        }
    }

    for (const std::string& line : toLog)
    {
        m_logFile << line;
    }

    m_logFile.flush();
}

LoggerImpl::LoggerImpl(const std::string& taskName)
    : m_taskName(taskName),
      m_logFile(generateLogfileName(taskName)),
      m_keepLogging(true)
{
    if (m_logger)
    {
        throw LoggerAlreadySet("You must create exactly one Logger object "
                               "per executable (or at least at one time).");
    }
    m_logger = this;

    auto ioJob = [this]()
    {
        while (m_keepLogging)
        {
            logSome(LINES_TO_LOG_AT_ONCE);

            int64_t delayMs = std::max(
                (ssize_t) MIN_DELAY,
                BASE_DELAY - WEIGHT_PER_LINE_FOR_DELAY * std::ssize(m_toLog) );

            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
    };

    m_ioThread = std::thread(ioJob);
}

LoggerImpl::~LoggerImpl()
{
    m_keepLogging = false;
    m_ioThread.join();
    
    logSome(m_toLog.size());

    m_logFile.close();
    m_logger = nullptr;
}

}
