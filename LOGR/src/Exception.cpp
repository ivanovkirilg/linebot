#include "LOGR/Exception.hpp"

#include <atomic>
#include <source_location>
#include <stdexcept>
#include <string>

#include "LOGR/internal.hpp"


std::atomic<unsigned long long> LOGR::Exception::freeId = 1;

LOGR::Exception::Exception(const std::string& message,
    const std::source_location& loc)
    : std::runtime_error(message), id(freeId++)
{
    internal::logLinePrefix(internal::Severity::EXCEPTION, loc);
    internal::logfile << "> [" << id << "] " << this->what() << "\n";
}

void LOGR::Exception::handle(const std::string& message,
    const std::source_location& loc) const
{
    internal::logLinePrefix(internal::Severity::EXCEPTION, loc);
    internal::logfile << "< [" << id << "] " << message << "\n";
}
