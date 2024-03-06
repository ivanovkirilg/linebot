#include "UI/UserInterface.hpp"

#include <cmath>
#include <iostream>
#include <exception>


static constexpr size_t WIDTH = 80;


static void draw(std::weak_ptr<const IDriver> driver, std::ostream& output)
{
    std::shared_ptr<const IDriver> dr = driver.lock();
    if (dr)
    {
        output << '\r' << '|';
        for (size_t i = 0; i < WIDTH; i++)
        {
            if (std::round(dr->position() * WIDTH) == i)
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
}

void UserInterface::run(
        std::chrono::milliseconds refreshRate,
        std::weak_ptr<const IDriver> driver)
{
    auto job = [this, refreshRate, driver]()
    {
        while (m_running && not driver.expired())
        {
            if (not m_paused)
            {
                draw(driver, std::cout);
            }

            std::this_thread::sleep_for(refreshRate);
        }

        m_running = false;
    };

    m_running = true;
    m_background = std::thread(job);
}

void UserInterface::terminate()
{
    m_running = false;
    m_background.join();
}


move::Move UserInterface::readMove()
{
    m_paused = true;

    std::cout << " Enter target position & speed: ";

    move::Move move;
    std::cin >> move.targetPosition >> move.speed;

    for (int retries = 0; not move::isValid(move); retries++)
    {
        if (retries > 5)
        {
            throw std::runtime_error("No valid move entered");
        }

        std::cout << " Enter target position [0, 1] & speed: ";
        std::cin >> move.targetPosition >> move.speed;
    }

    m_paused = false;

    return move;
}