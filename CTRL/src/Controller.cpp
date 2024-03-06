#include "CTRL/Controller.hpp"

#include "DOMN/Move.hpp"


void Controller::executeMove(const move::Move& move)
{
    // Stop driver
    m_driver->accelerate(-m_driver->velocity());

    m_driver->loggingOn();

    const double distance = std::abs(move.targetPosition - m_driver->position());
    const double moveTime = distance / move.speed;
    const double direction = 
        (move.targetPosition > m_driver->position())  ? 1 : -1;

    m_driver->accelerate(move.speed * direction);

    const long moveTimeMs = moveTime * 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(moveTimeMs));

    m_driver->accelerate(move.speed * -direction);

    m_driver->loggingOff();
}
