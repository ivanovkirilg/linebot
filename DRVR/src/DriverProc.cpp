#include "DriverProc.hpp"

#include "LOGR/Trace.hpp"

#include <iostream>
#include <thread>

using namespace DRVR;


double DriverProc::position()
{
    return m_position;
}

double DriverProc::velocity()
{
    return m_velocity;
}

void DriverProc::setVelocity(double velocity)
{
    LOGR::Trace trace{"current =", m_velocity, "new =", velocity};
    m_velocity = velocity;
    m_acceleration = 0.0;
}

void DriverProc::setAcceleration(double acceleration)
{
    LOGR::Trace trace{"current =", m_acceleration, "new =", acceleration};
    m_acceleration = acceleration;
}

void DriverProc::loggingOn()
{
    m_logFile << "Log " << m_logCount++ << '\n';
    m_logging = true;
}

void DriverProc::loggingOff()
{
    m_logging = false;
    m_logFile << std::endl;
}

void DriverProc::run(int refreshRate_ms)
{
    auto job = [this, refreshRate_ms]()
    {
        while (m_running)
        {
            const double deltaT = refreshRate_ms / 1000.0;
            m_velocity += m_acceleration * deltaT;
            m_position += m_velocity * deltaT;

            if (m_logging)
            {
                m_logFile << m_position << '\t';
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(refreshRate_ms));
        }
    };

    m_running = true;
    m_background = std::thread(job);
}

void DriverProc::terminate()
{
    m_running = false;
    m_background.join();
    m_breakLoop = true;
}
