#include "LOGR/Logger.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <thread>


static inline std::string generateLogfileName()
{
    std::stringstream stream;
    stream << "LOGR_" << std::this_thread::get_id() << ".csv";
    return stream.str();
}

thread_local std::ofstream LOGR::internal::logfile(generateLogfileName());
std::atomic<unsigned long long> LOGR::Exception::freeId = 1;

void LOGR::internal::logLinePrefix(
    Severity severity,
    const std::source_location& loc)
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto nowUs = duration_cast<microseconds>(now.time_since_epoch())
                 % microseconds(1s);

    std::time_t nowC = system_clock::to_time_t(now);

    std::ostringstream nowStr;

    nowStr << std::put_time(std::localtime(&nowC), "%F %T")
           << '.' << std::setfill('0') << std::setw(6) << nowUs.count();

    internal::logfile << static_cast<int>(severity) << SEPARATOR
                      << nowStr.str() << SEPARATOR
                      << loc.function_name() << SEPARATOR;
}

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
