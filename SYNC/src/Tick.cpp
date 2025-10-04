#include "SYNC/Tick.hpp"

#include "LOGR/Warning.hpp"

#include <thread>

using namespace SYNC;
using namespace std::chrono;

Tick::Tick(duration refreshRate)
    : m_refreshRate(refreshRate),
    m_currentTime(system_clock::now()),
    m_previousTime(m_currentTime),
    m_deltaTime()
{
}

void Tick::operator()()
{
    m_currentTime = system_clock::now();
    m_deltaTime = m_currentTime - m_previousTime;

    std::this_thread::sleep_for(m_refreshRate - m_deltaTime);
    m_previousTime = system_clock::now();
}

void StrictTick::operator()(const std::source_location& loc)
{
    Tick::operator()();

    if (m_deltaTime > m_refreshRate)
    {
        LOGR::Warning<const char*, const duration&, const char*, const duration&>
        {"Overran", m_refreshRate, "with delta time", m_deltaTime, loc};
    }
}
