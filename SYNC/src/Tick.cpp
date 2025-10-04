#include "SYNC/Tick.hpp"

#include "LOGR/Warning.hpp"

#include <fstream>
#include <sstream>
#include <thread>

using namespace SYNC;
using namespace std::chrono;

Tick::Tick(duration refreshRate)
    : m_refreshRate(refreshRate),
    m_currentTime(system_clock::now()),
    m_previousTime(m_currentTime),
    m_deltaTime(),
    m_deltaTimeIdx(0),
    m_lastDeltaTimes()
{
}

Tick::~Tick()
{
    std::ostringstream logfile_name;
    logfile_name << std::this_thread::get_id() << "_Tick_deltatimes";
    std::ofstream logfile{logfile_name.str()};

    for (auto& deltaTime : m_lastDeltaTimes)
    {
        logfile << deltaTime << '\n';
    }
}

void Tick::operator()()
{
    m_currentTime = system_clock::now();
    m_deltaTime = m_currentTime - m_previousTime;

    std::this_thread::sleep_for(m_refreshRate - m_deltaTime);
    m_previousTime = system_clock::now();

    m_lastDeltaTimes[m_deltaTimeIdx % NR_LAST_DELTA_TIMES] = m_deltaTime;
    m_deltaTimeIdx++;
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
