#include "Driver.hpp"

#include "LOGR/Trace.hpp"

#include <iostream>
#include <thread>


using namespace DRVR;


void Driver::accelerate(double instantaneousAcceleration)
{
    LOGR::Trace trace{instantaneousAcceleration};

    m_velocity = m_velocity + instantaneousAcceleration;

    trace.log("New velocity", m_velocity);
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
    using namespace std::chrono;

    LOGR::Trace trace;

    auto currentTime = system_clock::now();
    auto previousTime = currentTime;

    while (m_running)
    {
        currentTime = system_clock::now();

        const auto deltaTime_ms = duration_cast<milliseconds>(currentTime - previousTime);
        const double deltaTime_s = deltaTime_ms.count() / 1000.0;

        m_position = m_position + (m_velocity * deltaTime_s);

        if (m_logging)
        {
            m_logFile << m_position << '\t';
        }

        previousTime = currentTime;
        std::this_thread::sleep_for(milliseconds(refreshRate_ms));
    }
}

void Driver::terminate()
{
    m_running = false;
    m_background.join();

    m_breakLoop = true;
}
