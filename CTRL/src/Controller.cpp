#include "CTRL/Controller.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Trace.hpp"

#include <cmath>
#include <stdexcept>
#include <string>
#include <thread>


using namespace move;


void Controller::executeMove(const Move& move)
{
    switch (move.type) 
    {
        case MoveType::LINEAR:
            executeMove(std::get<LinearMove>(move.profile));
            break;
        case move::MoveType::TRIANGULAR:
            executeMove(std::get<TriangularMove>(move.profile));
            break;

        default:
            throw std::runtime_error("invalid move type " + std::to_string((int) move.type));
            break;
    };
}

void Controller::executeMove(const TriangularMove& move)
{
    (void) move;

    throw std::logic_error("executeMove() not implemented for triangular moves");
}

void Controller::executeMove(const LinearMove& move)
{
    LOGR::Trace trace(move.targetPosition, move.speed);
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
