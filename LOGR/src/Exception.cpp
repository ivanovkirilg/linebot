#include "LOGR/Exception.hpp"

#include <atomic>
#include <source_location>
#include <stdexcept>
#include <string>

#include <cerrno>
#include <cstring>

#include "LOGR/internal.hpp"
#include "LoggerImpl.hpp"


std::atomic<unsigned long long> LOGR::Exception::freeId = 1;

LOGR::Exception::Exception(const std::string& message,
    const std::source_location& loc)
    : std::runtime_error(message), m_id(freeId++)
{
    auto line = internal::startLine(internal::Level::EXCEPTION, loc);
    line << "> [" << m_id << "] " << this->what() << "\n";
    LoggerImpl::instance()->queueLogLine(line.str());
}

void LOGR::Exception::handle(const std::string& message,
    const std::source_location& loc) const
{
    auto line = internal::startLine(internal::Level::EXCEPTION, loc);
    line << "< [" << m_id << "] " << message << "\n";
    LoggerImpl::instance()->queueLogLine(line.str());
}

long long LOGR::Exception::id() const
{
    return m_id;
}

/// Get a string representation of the current value and meaning of errno.
std::string LOGR::getUnderlyingError()
{
    auto e = errno;
    return std::to_string(e) + ": " + ::strerror(e);
}

