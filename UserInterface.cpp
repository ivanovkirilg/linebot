#include "UserInterface.hpp"

#include <cmath>
#include <iostream>


static constexpr size_t WIDTH = 80;


static void printLine(double position, std::ostream& output)
{
    output << '\r' << '|';
    for (size_t i = 0; i < WIDTH; i++)
    {
        if (std::round(position * WIDTH) == i)
        {
            output << '+';
        }
        else
        {
            output << ' ';
        }
    }
    output << '|';
    output.flush();
}

void UserInterface::run(
        std::chrono::milliseconds refreshRate,
        std::weak_ptr<const Driver> driver)
{
    auto job = [this, refreshRate, driver]()
    {
        while (m_running)
        {
            auto dr = driver.lock();
            if (dr)
            {
                printLine(dr->position(), std::cout);
            }
            else
            {
                break;
            }

            std::this_thread::sleep_for(refreshRate);
        }
    };

    m_running = true;
    m_background = std::thread(job);
}

void UserInterface::join()
{
    m_running = false;
    m_background.join();
}


Move UserInterface::readMove()
{
    Move move;
    std::cin >> move.targetPosition >> move.speed;

    return move;
}