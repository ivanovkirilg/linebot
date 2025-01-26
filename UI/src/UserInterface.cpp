#include "UI/UserInterface.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Trace.hpp"
#include "LOGR/Warning.hpp"

#include <cmath>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>

using namespace UI;

namespace
{

static constexpr size_t WIDTH = 80;

static constexpr const char* LINEAR_MOVE_PROMPT     = " Enter target position & speed: ";
static constexpr const char* TRIANGULAR_MOVE_PROMPT = " Enter target position & acceleration: ";


struct MoveInput
{
    enum class State
    {
        END_OF_INPUT,
        INVALID_INPUT,
        VALID_INPUT
    } state{};

    DOMN::MoveType type{};
    DOMN::Move move{};
    const char* profilePrompt;

    void tryReadMoveType()
    {
        LOGR::Trace trace;

        std::string line;
        if (not std::getline(std::cin, line))
        {
            state = State::END_OF_INPUT;
            return;
        }

        if (line == "l" or line == "linear")
        {
            state = State::VALID_INPUT;
            move.type = DOMN::MoveType::LINEAR;
            profilePrompt = LINEAR_MOVE_PROMPT;
        }
        else if (line == "t" or line == "triangular")
        {
            state = State::VALID_INPUT;
            move.type = DOMN::MoveType::TRIANGULAR;
            profilePrompt = TRIANGULAR_MOVE_PROMPT;
        }
        else
        {
            LOGR::Warning("Received invalid move type", line);
            state = State::INVALID_INPUT;
        }
    }

    void tryReadMoveProfile()
    {
        LOGR::Trace trace;

        std::string line;
        if (not std::getline(std::cin, line))
        {
            state = State::END_OF_INPUT;
            return;
        }

        switch (move.type)
        {
            case DOMN::MoveType::LINEAR:
                tryParseLinearMove(line);
                break;

            case DOMN::MoveType::TRIANGULAR:
                tryParseTriangularMove(line);
                break;

            default:
                // TODO
                break;
        }
    }

    void retry(int retries, std::function<void()> method)
    {
        method();

        for (int retry = 0; state == MoveInput::State::INVALID_INPUT; retry++)
        {
            if (retry > retries)
            {
                throw InvalidInputException("No valid move entered (retried)");
            }

            method();
        }
    }

private:
    void tryParseLinearMove(const std::string& line)
    {
        DOMN::LinearMove linearMove;
        std::istringstream lineStream(line);
        lineStream >> linearMove.targetPosition >> linearMove.speed;

        if (DOMN::isValid(linearMove))
        {
            state = State::VALID_INPUT;
            move.profile = linearMove;
        }
        else
        {
            state = State::INVALID_INPUT;
            LOGR::Warning("Received invalid move profile ", linearMove.targetPosition, linearMove.speed);
        }
    }

    void tryParseTriangularMove(const std::string& line)
    {
        DOMN::TriangularMove triangularMove;
        std::istringstream lineStream(line);
        lineStream >> triangularMove.targetPosition >> triangularMove.acceleration;

        if (DOMN::isValid(triangularMove))
        {
            state = State::VALID_INPUT;
            move.profile = triangularMove;
        }
        else
        {
            state = State::INVALID_INPUT;
            LOGR::Warning("Received invalid move profile ", triangularMove.targetPosition, triangularMove.acceleration);
        }
    }
};

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

    MoveInput input;

    std::cout << " Choose move type: linear(l), triangular(t): ";
    input.retry(3, std::bind(&MoveInput::tryReadMoveType, &input));

    if (input.state == MoveInput::State::VALID_INPUT)
    {
        std::cout << input.profilePrompt;
    }
    input.retry(3, std::bind(&MoveInput::tryReadMoveProfile, &input));

    if (input.state == MoveInput::State::VALID_INPUT)
    {
        return input.move;
    }
    else
    {
        return std::nullopt;
    }
}
