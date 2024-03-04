#ifndef SRC_MOVE
#define SRC_MOVE

namespace move
{

constexpr double MIN_POSITION = 0.0;
constexpr double MAX_POSITION = 1.0;

constexpr double MIN_SPEED_EXCL = 0.0;


struct Move
{
    double targetPosition{};
    double speed{};
};

inline bool isValid(const Move& move)
{
    return (move.targetPosition >= MIN_POSITION)
        && (move.targetPosition <= MAX_POSITION)
        && (move.speed > MIN_SPEED_EXCL);
}

}

#endif // SRC_MOVE
