#include "MoveInput.hpp"

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
        move.type = DOMN::MoveType::LINEAR;
        m_profilePrompt = LINEAR_MOVE_PROMPT;
    }
    else if (line == "t" or line == "triangular")
    {
        m_state = State::VALID_INPUT;
        move.type = DOMN::MoveType::TRIANGULAR;
        m_profilePrompt = TRIANGULAR_MOVE_PROMPT;
    }
    else
    {
        LOGR::Warning("Received invalid move type", line);
        m_state = State::INVALID_INPUT;
    }
}

void MoveInput::tryReadMoveProfile()
{
    LOGR::Trace trace;

    std::string line;
    if (not std::getline(m_inputStream, line))
    {
        m_state = State::END_OF_INPUT;
        return;
    }

    switch (move.type)
    {
        case DOMN::MoveType::LINEAR:
            tryParseMoveProfile<DOMN::LinearMove, &DOMN::LinearMove::speed>(line);
            break;

        case DOMN::MoveType::TRIANGULAR:
            tryParseMoveProfile<DOMN::TriangularMove, &DOMN::TriangularMove::acceleration>(line);
            break;

        default:
            // TODO
            break;
    }
}

template<class SpecificMove, double SpecificMove::*profileCharacteristic>
void MoveInput::tryParseMoveProfile(const std::string& line)
{
    SpecificMove parsed;
    std::istringstream lineStream(line);
    lineStream >> parsed.targetPosition >> parsed.*profileCharacteristic;

    if (DOMN::isValid(parsed))
    {
        m_state = State::VALID_INPUT;
        move.profile = parsed;
    }
    else
    {
        m_state = State::INVALID_INPUT;
        LOGR::Warning("Received invalid move profile ",
                      parsed.targetPosition,
                      parsed.*profileCharacteristic);
    }
}
