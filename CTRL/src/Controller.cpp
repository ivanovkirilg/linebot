#include "CTRL/Controller.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Trace.hpp"

#include <cmath>
#include <stdexcept>
#include <string>
#include <thread>


using namespace DOMN;


void Controller::executeMove(const Move& move)
{
    switch (move.type) 
    {
        case MoveType::LINEAR:
            executeMove(std::get<LinearMove>(move.profile));
            break;
        case MoveType::TRIANGULAR:
            executeMove(std::get<TriangularMove>(move.profile));
            break;

        default:
            throw std::runtime_error("invalid move type " + std::to_string((int) move.type));
            break;
    };
}

void Controller::executeMove(const TriangularMove& move)
{
    LOGR::Trace trace(move.targetPosition, move.acceleration);

    m_driver->loggingOn();

    const double forwards = 
        (move.targetPosition > m_driver->position())  ? 1 : -1;
    const double backwards = -forwards;
    const double distance = std::abs(move.targetPosition - m_driver->position());
    const double maxVelocityPoint = m_driver->position() + (distance / 2) * forwards;

    m_driver->setAcceleration(move.acceleration * forwards);

    if (forwards > 0)
    {
        while (m_driver->position() < maxVelocityPoint)
        { }
        m_driver->setAcceleration(move.acceleration * backwards);

        while (m_driver->position() < move.targetPosition)
        {
            if (std::abs(m_driver->velocity()) < 0.01)
            {
                m_driver->accelerateInstantly(0.01 * forwards);
            }
        }
    }
    else
    {
        while (m_driver->position() > maxVelocityPoint)
        { }
        m_driver->setAcceleration(move.acceleration * backwards);

        while (m_driver->position() > move.targetPosition)
        {
            if (std::abs(m_driver->velocity()) < 0.01)
            {
                m_driver->accelerateInstantly(0.01 * forwards);
            }
        }
    }

    m_driver->setAcceleration(0.0);
    m_driver->accelerateInstantly(-m_driver->velocity()); // Stop

    m_driver->loggingOff();
}

void Controller::executeMove(const LinearMove& move)
{
    LOGR::Trace trace(move.targetPosition, move.speed);
    // Stop driver
    m_driver->accelerateInstantly(-m_driver->velocity());

    m_driver->loggingOn();

    const double distance = std::abs(move.targetPosition - m_driver->position());
    const double moveTime = distance / move.speed;
    const double direction = 
        (move.targetPosition > m_driver->position())  ? 1 : -1;

    m_driver->accelerateInstantly(move.speed * direction);

    const long moveTimeMs = moveTime * 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(moveTimeMs));

    m_driver->accelerateInstantly(move.speed * -direction);

    m_driver->loggingOff();
}
