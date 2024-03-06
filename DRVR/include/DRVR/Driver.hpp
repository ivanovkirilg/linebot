#ifndef DRVR_INCLUDE_DRVR_DRIVER
#define DRVR_INCLUDE_DRVR_DRIVER
#ifndef SRC_WAFER_STAGE_DRIVER
#define SRC_WAFER_STAGE_DRIVER

#include <atomic>
#include <chrono>
#include <thread>
#include <fstream>


class Driver
{
public:
    Driver(double position)
    :   m_position(position),
        m_logFile( std::string("driver") + std::to_string(time(NULL)) + ".txt" )
    { }

    inline double position() const { return m_position; }
    inline double velocity() const { return m_velocity; }

    void loggingOn();
    void loggingOff();

    void run(std::chrono::milliseconds refreshRate);
    void terminate();

    void accelerate(double instantaneousAcceleration);

private:
    std::atomic<double> m_position{};
    std::atomic<double> m_velocity{};

    std::atomic<bool> m_logging{};
    std::ofstream m_logFile;
    long m_logCount{};

    std::atomic<bool> m_running{};
    std::thread m_background;
};

#endif // SRC_WAFER_STAGE_DRIVER


#endif // DRVR_INCLUDE_DRVR_DRIVER
