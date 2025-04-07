#ifndef UI_INC_MOVE_INPUT
#define UI_INC_MOVE_INPUT

#include "UI/UserInterface.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Warning.hpp"

#include <sstream>


class MoveInput
{
public:
    MoveInput(std::istream& inputStream);
    DOMN::Move move{};
    const char* profilePrompt;

    bool valid() const;

    void tryReadMoveType();
    void tryReadMoveProfile();

    template <void (MoveInput::*method)()>
    void retry(int retries)
    {
        (this->*method)();

        for (int retry = 0; state == MoveInput::State::INVALID_INPUT; retry++)
        {
            if (retry >= retries)
            {
                throw UI::InvalidInputException("No valid move entered (retried)");
            }

            (this->*method)();
        }
    }

private:
    enum class State
    {
        END_OF_INPUT,
        INVALID_INPUT,
        VALID_INPUT
    } state{};
    std::istream& m_inputStream;

    template<class SpecificMove, double SpecificMove::*profileCharacteristic>
    void tryParseMoveProfile(const std::string& line)
    {
        SpecificMove parsed;
        std::istringstream lineStream(line);
        lineStream >> parsed.targetPosition >> parsed.*profileCharacteristic;

        if (DOMN::isValid(parsed))
        {
            state = State::VALID_INPUT;
            move.profile = parsed;
        }
        else
        {
            state = State::INVALID_INPUT;
            LOGR::Warning("Received invalid move profile ",
                          parsed.targetPosition,
                          parsed.*profileCharacteristic);
        }
    }
};

#endif // UI_INC_MOVE_INPUT
