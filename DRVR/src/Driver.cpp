#include "Driver.hpp"

#include "LOGR/Trace.hpp"

#include <iostream>
#include <thread>

using namespace DRVR;


double Driver::position()
{
    return m_position;
}

double Driver::velocity()
{
    return m_velocity;
}

void Driver::setVelocity(double velocity)
{
    LOGR::Trace trace{"current =", m_velocity, "new =", velocity};
    m_velocity = velocity;
    m_acceleration = 0.0;
}

void Driver::setAcceleration(double acceleration)
{
    LOGR::Trace trace{"current =", m_acceleration, "new =", acceleration};
    m_acceleration = acceleration;
}

void Driver::loggingOn()
{
    LOGR::Trace trace{m_logCount};

    m_logFile << "Log " << m_logCount++ << '\n';
    m_logging = true;
}

void Driver::loggingOff()
{
    LOGR::Trace trace{m_logCount};

    m_logging = false;
    m_logFile << std::endl;
}

void Driver::run(int refreshRate_ms)
{
    LOGR::Trace trace{refreshRate_ms};

    if (m_running)
    {
        throw std::runtime_error{"already running"};
    }

    m_running = true;
    m_background = std::thread(&Driver::simulate, this, refreshRate_ms);
}

void Driver::simulate(int refreshRate_ms)
{
    LOGR::Trace trace;

    while (m_running)
    {
        const double deltaT = refreshRate_ms / 1000.0;
        m_velocity += m_acceleration * deltaT;
        m_position += m_velocity * deltaT;

        if (m_logging)
        {
            m_logFile << m_position << '\t';
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(refreshRate_ms));
    }
}

void Driver::terminate()
{
    LOGR::Trace trace{"running before =", m_running};

    m_running = false;
    m_background.join();
    m_breakLoop = true;
}
