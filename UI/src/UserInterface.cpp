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
    auto job = [this, refreshRate, driver]()
    {
        while (m_running && not driver.expired())
        {
            if (not m_paused)
            {
                ScopedOn outputPause(m_paused);
                draw(driver, std::cout);
            }

            std::this_thread::sleep_for(refreshRate);
        }

        m_running = false;
    };

    m_running = true;
    m_background = std::thread(job);
    // FIXME waiting for background to start
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
    ScopedOn outputPause(m_paused);

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