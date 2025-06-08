#include "DRVR/Driver.hpp"

double Driver::position()
{
    return m_client.position();
}

double Driver::velocity()
{
    return m_client.velocity();
}

void Driver::setVelocity(double velocity)
{
    m_client.setVelocity(velocity);
}

void Driver::setAcceleration(double acceleration)
{
    m_client.setAcceleration(acceleration);
}

void Driver::loggingOn()
{
    m_client.loggingOn();
}

void Driver::loggingOff()
{
    m_client.loggingOff();
}

void Driver::run(std::chrono::milliseconds refreshRate)
{
    m_client.run(refreshRate.count());
}

void Driver::terminate()
{
    m_client.terminate();
}
