#ifndef SYNC_INCLUDE_SYNC_TICK
#define SYNC_INCLUDE_SYNC_TICK

#include <chrono>
#include <source_location>

namespace SYNC
{

class Tick
{
public:
    static constexpr auto MAX_DRIFT = std::chrono::microseconds(500);

    using clock = std::chrono::steady_clock;

    Tick(clock::duration refreshRate);

    void operator()();

    inline clock::duration deltaTime() { return std::max(m_deltaTime, m_refreshRate); }

protected:
    const clock::duration m_refreshRate;
    clock::time_point m_endOfLastTick;
    clock::duration m_deltaTime;
};

class StrictTick : public Tick
{
public:
    using Tick::Tick;

    void operator()(const std::source_location& loc = std::source_location::current());
};

} // SYNC

#endif // SYNC_INCLUDE_SYNC_TICK
