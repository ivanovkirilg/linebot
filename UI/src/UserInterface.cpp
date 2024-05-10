#include "UI/UserInterface.hpp"

#include <cmath>
#include <iostream>
#include <sstream>

using namespace UI;

namespace
{

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

class ScopedOn
{
public:
    ScopedOn(std::atomic<bool>& var) : m_var(var)
    {
        m_var = true;
    }
    ~ScopedOn()
    {
        m_var = false;
    }

private:
    std::atomic<bool>& m_var;
};

} // namespace

void UserInterface::run(
        std::chrono::milliseconds refreshRate,
        std::weak_ptr<const IDriver> driver)
{
    m_background = std::thread([this, refreshRate, driver]()
    {
        draw(driver, std::cout);

        m_running = true;
        m_running.notify_one();

        while (m_running && not driver.expired())
        {
            if (m_outputMutex.try_lock())
            {
                std::lock_guard outLock(m_outputMutex, std::adopt_lock);
                draw(driver, std::cout);
            }

            std::this_thread::sleep_for(refreshRate);
        }

        m_running = false;
    });

    m_running.wait(false);
}

void UserInterface::terminate()
{
    m_running = false;
    m_background.join();
}

static move::Move tryReadMove()
{
    std::string line;
    if (not std::getline(std::cin, line))
    {
        throw EndOfInputException("Could not read line");
    }

    move::Move move;
    std::istringstream lineStream(line);
    lineStream >> move.targetPosition >> move.speed;

    return move;
}

move::Move UserInterface::readMove()
{
    std::lock_guard outLock(m_outputMutex);

    std::cout << " Enter target position & speed: ";

    move::Move move = tryReadMove();

    for (int retries = 0; not move::isValid(move); retries++)
    {
        if (retries > 3)
        {
            throw InvalidInputException("No valid move entered (retried)");
        }

        std::cout << " Enter target position [0, 1] & speed: ";
        move = tryReadMove();
    }

    return move;
}