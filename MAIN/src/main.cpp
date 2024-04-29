#include "UI/UserInterface.hpp"
#include "DRVR/Driver.hpp"
#include "CTRL/Controller.hpp"

#include <chrono>
#include <iostream>


int main()
{
    using namespace std::chrono;

    auto driver = std::make_shared<Driver>(0.5);
    driver->run(milliseconds(4));

    Controller controller(driver);

    UI::UserInterface ui;
    ui.run(milliseconds(16), driver);

    try
    {
        while (true)
        {
            move::Move move = ui.readMove();

            controller.executeMove(move);
        }
    }
    catch (UI::EndOfInputException)
    {
        // TODO: This is actually a normal situation, probably shouldn't throw!
    }
    catch (UI::InvalidInputException)
    {
        std::cout << "\n" "Received too many invalid inputs, quitting."
                  << std::endl;
    }

    ui.terminate();

    driver->terminate();

    return 0;
}
