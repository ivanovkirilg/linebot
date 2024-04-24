#ifndef DRVR_INCLUDE_DRVR_DRIVER
#define DRVR_INCLUDE_DRVR_DRIVER

#include "IDriver.hpp"

#include <atomic>
#include <chrono>
#include <thread>
#include <fstream>


class Driver : public IDriver
{
public:
    Driver(double position)
    :   m_position(position),
        m_logFile( std::string("driver") + std::to_string(time(NULL)) + ".txt" )
    { }

    virtual double position() const override { return m_position; }
    virtual double velocity() const override { return m_velocity; }

    virtual void loggingOn() override;
    virtual void loggingOff() override;

    virtual void run(std::chrono::milliseconds refreshRate) override;
    virtual void terminate() override;

    virtual void accelerate(double instantaneousAcceleration) override;

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
