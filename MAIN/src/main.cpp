#include "UI/UserInterface.hpp"
#include "DRVR/Driver.hpp"
#include "CTRL/Controller.hpp"
#include "LOGR/ILogger.hpp"
#include "LOGR/Trace.hpp"

#include <memory>
#include <iostream>


int main(int argc, char *argv[])
{
    using namespace std::literals;

    if (argc != 2)
    {
        std::cout << "Provide exactly 1 argument: port for Driver interface\n";
        return 1;
    }

    int port = std::atoi(argv[1]);

    auto logger = LOGR::ILogger::create("MAIN");

    LOGR::Trace trace;

    auto driver = std::make_shared<Driver>(port);
    driver->run(4ms);

    Controller controller(driver);

    UI::UserInterface ui;
    ui.run(16ms, driver);

    try
    {
        std::optional<DOMN::Move> move = ui.readMove();

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
