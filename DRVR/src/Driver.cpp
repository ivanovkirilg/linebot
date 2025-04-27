#include "DRVR/Driver.hpp"

#include "LOGR/Trace.hpp"

#include <iostream>
#include <thread>

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
    m_logFile << "Log " << m_logCount++ << '\n';
    m_logging = true;
}

void Driver::loggingOff()
{
    m_logging = false;
    m_logFile << std::endl;
}

void Driver::run(std::chrono::milliseconds refreshRate)
{
    auto job = [this, refreshRate]()
    {
        while (m_running)
        {
            const double deltaT = refreshRate.count() / 1000.0;
            m_velocity += m_acceleration * deltaT;
            m_position += m_velocity * deltaT;

            if (m_logging)
            {
                m_logFile << m_position << '\t';
            }

            std::this_thread::sleep_for(refreshRate);
        }
    };

    m_running = true;
    m_background = std::thread(job);
}

void Driver::terminate()
{
    m_running = false;
    m_background.join();
}
