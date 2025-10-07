#include "SYNC/Tick.hpp"

#include "LOGR/Warning.hpp"

#include <thread>

using namespace SYNC;
using namespace std::chrono;

Tick::Tick(duration refreshRate)
    : m_refreshRate(refreshRate),
    m_endOfLastTick(system_clock::now()),
    m_deltaTime()
{
}

void Tick::operator()()
{

    const time_point start = system_clock::now();
    const duration elapsed = start - m_endOfLastTick;

    const time_point target = start + (m_refreshRate - elapsed);
    std::this_thread::sleep_until(target);

    const time_point endOfTick = system_clock::now();

    m_deltaTime = endOfTick - m_endOfLastTick;
    m_endOfLastTick = endOfTick;
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
