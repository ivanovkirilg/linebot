#include "UI/UserInterface.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Trace.hpp"
#include "LOGR/Warning.hpp"

#include <cmath>
#include <iostream>
#include <optional>
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

} // namespace

void UserInterface::run(
        std::chrono::milliseconds refreshRate,
        std::weak_ptr<const IDriver> driver)
{
    LOGR::Trace trace;
    
    m_background = std::thread([this, refreshRate, driver]()
    {
        LOGR::Trace bgTrace;
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

static std::optional<move::LinearMove> tryReadMove()
{
    LOGR::Trace trace;
    std::string line;
    if (not std::getline(std::cin, line))
    {
        return std::nullopt;
    }

    move::LinearMove move;
    std::istringstream lineStream(line);
    lineStream >> move.targetPosition >> move.speed;

    return move;
}

std::optional<move::Move> UserInterface::readMove()
{
    LOGR::Trace trace;
    std::lock_guard outLock(m_outputMutex);

    std::cout << " Enter target position & speed: ";

    std::optional<move::LinearMove> move = tryReadMove();

    for (int retries = 0; move and not move::isValid(move.value()); retries++)
    {
        if (retries > 3)
        {
            throw InvalidInputException("No valid move entered (retried)");
        }

        LOGR::Warning("Received invalid move");

        std::cout << " Enter target position [0, 1] & speed: ";
        move = tryReadMove();
    }

    // FIXME temp until reading Triangular move is implemented
    if (move)
    {
        return move::Move{move::MoveType::LINEAR, move.value()};
    }
    else
    {
        return std::nullopt;
    }
}