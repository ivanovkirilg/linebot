#include "UserInterface.hpp"
#include "Driver.hpp"
#include "Controller.hpp"

#include <chrono>
#include <iostream>


int main()
{
    using namespace std::chrono;

    auto driver = std::make_shared<Driver>(0.5);
    driver->run(milliseconds(20));

    Controller controller(driver);

    UserInterface ui;
    ui.run(milliseconds(50), driver);

    while (true)
    {
        Move move = ui.readMove();

        controller.executeMove(move);
    }

    ui.join();

    driver->join();

    return 0;
}
