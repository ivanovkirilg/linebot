#include "SYNC/Tick.hpp"

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
