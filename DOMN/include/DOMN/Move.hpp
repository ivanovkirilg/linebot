#ifndef DOMN_INCLUDE_DOMN_MOVE
#define DOMN_INCLUDE_DOMN_MOVE

#include <ostream>
#include <variant>
#include <iostream>


namespace DOMN
{

constexpr double MIN_POSITION = 0.0;
constexpr double MAX_POSITION = 1.0;

constexpr double MIN_SPEED_EXCL = 0.0;
constexpr double MIN_ACCELERATION_EXCL = 0.0;

enum class MoveProfile
{
    LINEAR,
    TRIANGULAR
};

struct LinearProfile
{
    double speed{};
};

struct TriangularProfile
{
    double acceleration{};
};

struct Move 
{
    double targetPosition{};
    MoveProfile type{};
    std::variant<LinearProfile, TriangularProfile> profile{};
};

inline std::ostream& operator<<(std::ostream& os, const Move& move)
{
    os << "Move(targetPosition=" << move.targetPosition << ", ";

    switch (move.type)
    {
        break; case MoveProfile::LINEAR:
        os << "type=LINEAR, speed=" << std::get<LinearProfile>(move.profile).speed;
        break; case MoveProfile::TRIANGULAR:
        os << "type=TRIANGULAR, acceleration=" << std::get<TriangularProfile>(move.profile).acceleration;
        break; default:
        os << "type=!INVALID(" << static_cast<int>(move.type) << ")!";
    }
    return os << ')';
}

inline bool isValid(const Move& move)
{
    return (move.targetPosition >= MIN_POSITION)
        && (move.targetPosition <= MAX_POSITION)
        && (
            (
                (move.type == MoveProfile::LINEAR) &&
                (std::get<LinearProfile>(move.profile).speed > MIN_SPEED_EXCL)
            ) || (
                (move.type == MoveProfile::TRIANGULAR) &&
                (std::get<TriangularProfile>(move.profile).acceleration > MIN_ACCELERATION_EXCL)
            )
        );
}

} // DOMN

#endif // SRC_MOVE
