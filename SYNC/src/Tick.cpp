#include "SYNC/Tick.hpp"

#include "LOGR/Warning.hpp"

#include <thread>

using namespace SYNC;
using namespace std::chrono;

Tick::Tick(clock::duration refreshRate)
    : m_refreshRate(refreshRate),
    m_endOfLastTick(clock::now()),
    m_deltaTime()
{
}

void Tick::operator()()
{

    const time_point start = clock::now();
    const duration elapsed = start - m_endOfLastTick;

    const time_point target = start + (m_refreshRate - elapsed);
    std::this_thread::sleep_until(target);

    const time_point endOfTick = clock::now();

    m_deltaTime = endOfTick - m_endOfLastTick;
    m_endOfLastTick = endOfTick;
}

void StrictTick::operator()(const std::source_location& loc)
{
    Tick::operator()();

    if (m_deltaTime > (m_refreshRate + MAX_DRIFT))
    {
        LOGR::Warning<const char*, const clock::duration&, const char*, const clock::duration&>
        {"Overran", m_refreshRate, "with delta time", m_deltaTime, loc};
    }
}
