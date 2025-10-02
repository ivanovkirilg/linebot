#ifndef SYNC_INCLUDE_SYNC_TICK
#define SYNC_INCLUDE_SYNC_TICK

#include <chrono>

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

private:
    const duration m_refreshRate;
    time_point m_currentTime;
    time_point m_previousTime;
    duration m_deltaTime;
};

} // SYNC

#endif // SYNC_INCLUDE_SYNC_TICK
