#include "LOGR/Logger.hpp"

#include <fstream>
#include <sstream>
#include <thread>

static inline std::string generateLogfileName()
{
    std::stringstream stream;
    stream << "LOGR_" << std::this_thread::get_id() << "_trace.txt";
    return stream.str();
}

thread_local std::ofstream LOGR::internal::logfile(generateLogfileName());
