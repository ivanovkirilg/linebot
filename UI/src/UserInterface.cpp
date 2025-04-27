#include "UI/UserInterface.hpp"

#include "MoveInput.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Trace.hpp"

#include <cmath>
#include <iostream>
#include <optional>

using namespace UI;

namespace
{

static constexpr int NR_OF_RETRIES = 3;

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

std::optional<DOMN::Move> UserInterface::readMove()
{
    LOGR::Trace trace;
    std::lock_guard outLock(m_outputMutex);

    MoveInput input{std::cin};

    std::cout << " Choose move type: linear(l), triangular(t): ";
    input.retry<&MoveInput::tryReadMoveType>(NR_OF_RETRIES);

    if (input.valid())
    {
        std::cout << input.profilePrompt();
        input.retry<&MoveInput::tryReadMoveProfile>(NR_OF_RETRIES);
    }

    if (input.valid())
    {
        return input.move;
    }
    else
    {
        return std::nullopt;
    }
}
