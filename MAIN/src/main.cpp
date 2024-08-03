#include "UI/UserInterface.hpp"
#include "DRVR/Driver.hpp"
#include "CTRL/Controller.hpp"
#include "LOGR/Logger.hpp"
#include "LOGR/ILogger.hpp"
#include "LOGR/Trace.hpp"

#include <chrono>
#include <iostream>


int main()
{
    using namespace std::literals;
    auto logger = LOGR::ILogger::create("MAIN");

    LOGR::Trace trace;

    auto driver = std::make_shared<Driver>(0.5);
    driver->run(4ms);

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
