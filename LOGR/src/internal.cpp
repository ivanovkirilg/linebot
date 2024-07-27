#include "LOGR/internal.hpp"

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

void LOGR::internal::logLinePrefix(
    Severity severity,
    const std::source_location& loc)
{
    using namespace std::chrono;
    auto now = system_clock::now().time_since_epoch();
    auto nowSec  = duration_cast<seconds>(now);
    auto nowMicroSec = duration_cast<microseconds>(now) % 1s;

    std::ostringstream nowStr;

    nowStr << nowSec.count() << '.'
           << std::setfill('0') << std::setw(6) << nowMicroSec.count();

    internal::logfile << static_cast<int>(severity) << SEPARATOR
                      << nowStr.str() << SEPARATOR
                      << loc.function_name() << SEPARATOR;
}
