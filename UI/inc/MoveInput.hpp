#ifndef UI_INC_MOVE_INPUT
#define UI_INC_MOVE_INPUT

#include "UI/UserInterface.hpp"

#include "DOMN/Move.hpp"


class MoveInput
{
public:
    MoveInput(std::istream& inputStream);
    DOMN::Move move{};

    const char* profilePrompt() const;
    bool valid() const;

    void tryReadMoveType();
    void tryReadMoveProfile();

    template <void (MoveInput::*method)()>
    void retry(int retries)
    {
        (this->*method)();

        for (int retry = 0; m_state == MoveInput::State::INVALID_INPUT; retry++)
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
    };

    State m_state{};
    std::istream& m_inputStream;
    const char* m_profilePrompt = "";

    template<class SpecificMove, double SpecificMove::*profileCharacteristic>
    void tryParseMoveProfile(const std::string& line);
};

#endif // UI_INC_MOVE_INPUT
