#include "UserInterface.hpp"
#include "Driver.hpp"
#include "Controller.hpp"

#include <chrono>
#include <iostream>


int main()
{
    using namespace std::chrono;

    auto driver = std::make_shared<Driver>(0.5);
    driver->run(milliseconds(4));

    Controller controller(driver);

    UserInterface ui;
    ui.run(milliseconds(16), driver);

    // FIXME Workaround for background starting slower than first readMove
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    while (true)
    {
        move::Move move = ui.readMove();

        controller.executeMove(move);
    }

    ui.terminate();

    driver->terminate();

    return 0;
}
