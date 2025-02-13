#include "Driver.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/Trace.hpp"
#include "LOGR/Warning.hpp"

#include <iostream>
#include <thread>


using namespace DRVR;


void Driver::accelerate(double instantaneousAcceleration)
{
    LOGR::Trace trace{instantaneousAcceleration};

    m_velocity = m_velocity + instantaneousAcceleration;

    trace.log("New velocity", m_velocity);
}

void Driver::position(double& pos)
{
    LOGR::Trace trace{m_position};

    pos = m_position;
}

void Driver::velocity(double& vel)
{
    LOGR::Trace trace{m_velocity};

    vel = m_velocity;
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

        if ((m_position < move::MIN_POSITION) || (m_position > move::MAX_POSITION))
        {
            LOGR::Warning{"Position", m_position, "went out of bounds", move::MIN_POSITION, move::MAX_POSITION};

            double pos = m_position;
            m_position = std::clamp(pos, move::MIN_POSITION, move::MAX_POSITION);
            m_velocity = 0.0;
        }

        if (m_logging)
        {
            m_logFile << m_position << '\t';
        }

        previousTime = currentTime;
        std::this_thread::sleep_for(milliseconds(refreshRate_ms));
    }
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

void Driver::terminate()
{
    m_running = false;
    m_background.join();

    m_breakLoop = true;
}
