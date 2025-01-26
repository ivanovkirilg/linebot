#ifndef DOMN_INCLUDE_DOMN_MOVE
#define DOMN_INCLUDE_DOMN_MOVE

#include <variant>


namespace DOMN
{

constexpr double MIN_POSITION = 0.0;
constexpr double MAX_POSITION = 1.0;

constexpr double MIN_SPEED_EXCL = 0.0;

enum class MoveType
{
    LINEAR,
    TRIANGULAR
};

struct LinearMove
{
    double targetPosition{};
    double speed{};
};

struct TriangularMove
{
    double targetPosition{};
    double acceleration{};
};

struct Move 
{
    MoveType type;
    std::variant<LinearMove, TriangularMove> profile;
};

inline bool isValid(const LinearMove& move)
{
    return (move.targetPosition >= MIN_POSITION)
        && (move.targetPosition <= MAX_POSITION)
        && (move.speed > MIN_SPEED_EXCL);
}

} // DOMN

#endif // SRC_MOVE
