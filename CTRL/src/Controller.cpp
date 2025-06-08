#include "CTRL/Controller.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Exception.hpp"
#include "LOGR/Trace.hpp"

#include <cmath>
#include <functional>
#include <string>
#include <thread>


using namespace DOMN;


namespace
{

class LogContext
{
public:
    LogContext(std::shared_ptr<DRVR::IDriverClient> driver)
        : m_driver(driver)
    {
        if (m_driver != nullptr)
        {
            m_driver->loggingOn();
        }
    }

    ~LogContext()
    {
        if (m_driver != nullptr)
        {
            m_driver->loggingOff();
        }
    }

private:
    std::shared_ptr<DRVR::IDriverClient> m_driver;
};

} // namespace

void Controller::executeMove(const Move& move)
{
    LogContext context{m_driver};

    switch (move.type)
    {
        case MoveType::LINEAR:
            executeMove(std::get<LinearMove>(move.profile));
            break;
        case MoveType::TRIANGULAR:
            executeMove(std::get<TriangularMove>(move.profile));
            break;

        default:
            throw LOGR::Exception("Invalid move type " + std::to_string((int) move.type));
            break;
    };
}

void Controller::executeMove(const TriangularMove& move)
{
    LOGR::Trace trace(move.targetPosition, move.acceleration);

    const double forwards = getDirection(move.targetPosition);
    const double backwards = -forwards;
    const double distance = getDistance(move.targetPosition);
    const double maxVelocityPoint = m_driver->position() + (distance / 2) * forwards;

    using compare = std::function<bool (double)>;
    auto notReached = (forwards > 0)
        ? (compare) [this](double target) { return m_driver->position() < target; }
        : (compare) [this](double target) { return m_driver->position() > target; };

    m_driver->setAcceleration(move.acceleration * forwards);

    while (notReached(maxVelocityPoint))
    {
        // Wait
    }

    m_driver->setAcceleration(move.acceleration * backwards);

    while (notReached(move.targetPosition))
    {
        const double minimumSpeed = 0.01; // Arbitrary

        if (std::abs(m_driver->velocity()) < minimumSpeed)
        {
            m_driver->setVelocity(minimumSpeed * forwards);
        }
    }

    m_driver->setVelocity(0.0);
}

void Controller::executeMove(const LinearMove& move)
{
    LOGR::Trace trace(move.targetPosition, move.speed);

    m_driver->setVelocity(0.0);

    const double distance = getDistance(move.targetPosition);
    const double moveTime = distance / move.speed;
    const double direction = getDirection(move.targetPosition);

    trace.log("Start");
    m_driver->setVelocity(move.speed * direction);

    const long moveTimeMs = moveTime * 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(moveTimeMs));
    
    trace.log("Stop");
    m_driver->setVelocity(0.0);
}

double Controller::getDirection(double targetPosition) const
{
    return (targetPosition > m_driver->position()) ? 1 : -1;
}

double Controller::getDistance(double targetPosition) const
{
    return std::abs(targetPosition - m_driver->position());
}
