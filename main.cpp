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

    UserInterface ui;
    ui.run(milliseconds(50), driver);

    //

    std::this_thread::sleep_for(milliseconds(500));

    driver->loggingOn();
    driver->accelerate(.2);
    std::this_thread::sleep_for(milliseconds(1000));
    driver->loggingOff();

    driver->loggingOn();
    driver->accelerate(-.6);
    std::this_thread::sleep_for(milliseconds(1000));
    driver->loggingOff();

    driver->accelerate(.4);

    std::this_thread::sleep_for(milliseconds(1000));

    //

    ui.join();

    driver->join();

    return 0;
}
