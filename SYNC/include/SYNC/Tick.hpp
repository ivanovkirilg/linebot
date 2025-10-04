#ifndef SYNC_INCLUDE_SYNC_TICK
#define SYNC_INCLUDE_SYNC_TICK

#include <chrono>
#include <source_location>

namespace SYNC
{

class Tick
{
public:
    using duration = std::chrono::system_clock::duration;
    using time_point = std::chrono::system_clock::time_point;

    Tick(duration refreshRate);

    void operator()();

    inline duration deltaTime() { return std::max(m_deltaTime, m_refreshRate); }

protected:
    const duration m_refreshRate;
    time_point m_currentTime;
    time_point m_previousTime;
    duration m_deltaTime;
};

class StrictTick : public Tick
{
public:
    using Tick::Tick;

    void operator()(const std::source_location& loc = std::source_location::current());
};

} // SYNC

#endif // SYNC_INCLUDE_SYNC_TICK
