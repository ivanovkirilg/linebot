#include "Driver.hpp"

#include <iostream>
#include <thread>


using namespace DRVR;


void Driver::accelerate(double instantaneousAcceleration)
{
    m_velocity = m_velocity + instantaneousAcceleration;
}

double Driver::position()
{
    return m_position;
}

double Driver::velocity()
{
    return m_velocity;\
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

void Driver::run(int refreshRate_ms)
{
    if (m_running)
    {
        throw std::runtime_error{"already running"};
    }

    auto job = [this, refreshRate_ms]()
    {
        while (m_running)
        {
            const double deltaT = refreshRate_ms / 1000.0;
            m_position = m_position + (m_velocity * deltaT);

            if (m_logging)
            {
                m_logFile << m_position << '\t';
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(refreshRate_ms));
        }
    };

    m_running = true;
    m_background = std::thread(job);
}

void Driver::terminate()
{
    m_running = false;
    m_background.join();

    m_breakLoop = true;
}
