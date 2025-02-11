#include "UI/UserInterface.hpp"
#include "DRVR/DriverClient.hpp"
#include "CTRL/Controller.hpp"
#include "LOGR/ILogger.hpp"
#include "LOGR/Trace.hpp"

#include <chrono>
#include <iostream>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Provide exactly 1 argument: port for Driver interface\n";
        return 1;
    }

    int driverPort = std::atoi(argv[1]);

    using namespace std::literals;
    auto logger = LOGR::ILogger::create("MAIN");

    LOGR::Trace trace;

    auto driver = std::make_shared<DRVR::DriverClient>(driverPort);
    try
    {
        driver->run(4);
    }
    catch (std::runtime_error& exc)
    {
        std::cout << "Driver run() failed, perhaps already running?\n";
    }

    Controller controller(driver);

    UI::UserInterface ui;
    ui.run(16ms, driver);

    try
    {
        std::optional<move::Move> move = ui.readMove();

        while (move.has_value())
        {
            controller.executeMove(move.value());

            move = ui.readMove();
        }
    }
    catch (const UI::InvalidInputException& exc)
    {
        exc.handle("Quitting application");

        std::cout << "\nReceived too many invalid inputs, quitting."
                  << std::endl;
    }

    ui.terminate();

    driver->terminate();

    return 0;
}
