#include "MoveInput.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Trace.hpp"
#include "LOGR/Warning.hpp"

#include <iostream>

namespace
{

static constexpr const char* LINEAR_MOVE_PROMPT     = " Enter target position & speed: ";
static constexpr const char* TRIANGULAR_MOVE_PROMPT = " Enter target position & acceleration: ";

}

MoveInput::MoveInput(std::istream& inputStream)
    : m_inputStream(inputStream)
{
};

bool MoveInput::valid() const
{
    return m_state == State::VALID_INPUT;
}

const char* MoveInput::profilePrompt() const
{
    return m_profilePrompt;
}

void MoveInput::tryReadMoveType()
{
    LOGR::Trace trace;

    std::string line;
    if (not std::getline(m_inputStream, line))
    {
        m_state = State::END_OF_INPUT;
        return;
    }

    if (line == "l" or line == "linear")
    {
        m_state = State::VALID_INPUT;
        move.type = DOMN::MoveProfile::LINEAR;
        m_profilePrompt = LINEAR_MOVE_PROMPT;
    }
    else if (line == "t" or line == "triangular")
    {
        m_state = State::VALID_INPUT;
        move.type = DOMN::MoveProfile::TRIANGULAR;
        m_profilePrompt = TRIANGULAR_MOVE_PROMPT;
    }
    else
    {
        LOGR::Warning("Received invalid move type", line);
        m_state = State::INVALID_INPUT;
    }
}

void MoveInput::tryReadMove()
{
    LOGR::Trace trace;

    std::string line;
    if (not std::getline(m_inputStream, line))
    {
        m_state = State::END_OF_INPUT;
        return;
    }

    std::istringstream lineStream{line};

    DOMN::Move parsed;
    parsed.type = move.type;

    lineStream >> parsed.targetPosition;

    if (parsed.type == DOMN::MoveProfile::LINEAR)
    {
        double speed{};
        lineStream >> speed;
        parsed.profile = DOMN::LinearProfile{speed};
    }
    else if (parsed.type == DOMN::MoveProfile::TRIANGULAR)
    {
        double acceleration{};
        lineStream >> acceleration;
        parsed.profile = DOMN::TriangularProfile{acceleration};
    }

    if (DOMN::isValid(parsed))
    {
        m_state = State::VALID_INPUT;
        move = parsed;
    }
    else
    {
        m_state = State::INVALID_INPUT;
        LOGR::Warning("Received invalid move:", parsed);
    }
}
