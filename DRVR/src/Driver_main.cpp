#include "DriverProc.hpp"

#include "LOGR/ILogger.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Provide exactly 1 argument: port for Driver interface\n";
        return 1;
    }

    int port = std::atoi(argv[1]);

    auto logger = LOGR::ILogger::create("DRVR");

    DRVR::DriverProc driver{0.5};

    driver.bind(port);

    try
    {
        driver.requestLoop();
    }
    catch (LOGR::Exception& exc)
    {
        driver.terminate();
        return -1;
    }
}
