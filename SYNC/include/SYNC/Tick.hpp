#ifndef SYNC_INCLUDE_SYNC_TICK
#define SYNC_INCLUDE_SYNC_TICK

#include <chrono>

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

private:
    const clock::duration m_refreshRate;
    clock::time_point m_endOfLastTick;
    clock::duration m_deltaTime;
};

} // SYNC

#endif // SYNC_INCLUDE_SYNC_TICK
