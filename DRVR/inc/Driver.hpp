#ifndef DRVR_INC_DRIVER_IMPL
#define DRVR_INC_DRIVER_IMPL

#include "DriverServer.hpp"

#include <fstream>
#include <thread>


namespace DRVR
{

class Driver : public DriverServer
{
public:
    Driver(double position)
    :   m_position(position),
        m_logFile( std::string("driver") + std::to_string(time(NULL)) + ".txt" )
    { }

protected:
    virtual void position(double& pos) override { pos = m_position; }
    virtual void velocity(double& vel) override { vel = m_velocity; }

    virtual void loggingOn() override;
    virtual void loggingOff() override;

    virtual void run(int refreshRate) override;
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

} // DRVR

#endif // DRVR_INC_DRIVER_IMPL
