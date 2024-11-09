#include "LOGR/Exception.hpp"

#include <atomic>
#include <source_location>
#include <stdexcept>
#include <string>

#include "LOGR/internal.hpp"
#include "LoggerImpl.hpp"


std::atomic<unsigned long long> LOGR::Exception::freeId = 1;

LOGR::Exception::Exception(const std::string& message,
    const std::source_location& loc)
    : std::runtime_error(message), id(freeId++)
{
    auto line = internal::startLine(internal::Level::EXCEPTION, loc);
    line << "> [" << id << "] " << this->what() << "\n";
    LoggerImpl::instance()->queueLogLine(line.str());
}

void LOGR::Exception::handle(const std::string& message,
    const std::source_location& loc) const
{
    auto line = internal::startLine(internal::Level::EXCEPTION, loc);
    line << "< [" << id << "] " << message << "\n";
    LoggerImpl::instance()->queueLogLine(line.str());
}
