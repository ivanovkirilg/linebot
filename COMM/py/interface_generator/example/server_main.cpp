// server-side hand-coded file

#include "Driver.hpp"

#include <cassert>

#include "LOGR/ILogger.hpp"

int main(int argc, char *argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);

    auto logger = LOGR::ILogger::create("XMPL_server");

    Driver driver{port};

    driver.requestLoop();

}
