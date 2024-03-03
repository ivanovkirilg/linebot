#ifndef SRC_MOVE
#define SRC_MOVE

namespace move
{

constexpr double MIN_POSITION = 0.0;
constexpr double MAX_POSITION = 1.0;

constexpr double MIN_SPEED = 0.0;


struct Move
{
    double targetPosition{};
    double speed{};
};

inline bool isValid(const Move& move)
{
    return (move.targetPosition >= 0.0)
        && (move.targetPosition <= 1.0)
        && (move.speed > 0.0);
}

}

#endif // SRC_MOVE
