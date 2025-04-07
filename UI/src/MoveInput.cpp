#include "MoveInput.hpp"

#include "LOGR/Trace.hpp"

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
    return state == State::VALID_INPUT;
}

void MoveInput::tryReadMoveType()
{
    LOGR::Trace trace;

    std::string line;
    if (not std::getline(m_inputStream, line))
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

void MoveInput::tryReadMoveProfile()
{
    LOGR::Trace trace;

    std::string line;
    if (not std::getline(m_inputStream, line))
    {
        state = State::END_OF_INPUT;
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
