#include "LOGR/internal.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>


std::ostringstream LOGR::internal::startLine(
    Severity severity,
    const std::source_location& loc)
{
    using namespace std::chrono;
    auto now = system_clock::now().time_since_epoch();
    auto nowSec  = duration_cast<seconds>(now);
    auto nowMicroSec = duration_cast<microseconds>(now) % 1s;

    std::ostringstream nowStr;

    nowStr << nowSec.count() << '.'
           << std::setfill('0') << std::setw(6)
           << nowMicroSec.count();

    std::ostringstream line;
    line << static_cast<int>(severity) << SEPARATOR
         << nowStr.str()               << SEPARATOR
         << std::this_thread::get_id() << SEPARATOR
         << loc.file_name()            << SEPARATOR
         << loc.line()                 << SEPARATOR
         << loc.function_name()        << SEPARATOR
         ;

    return line;
}
