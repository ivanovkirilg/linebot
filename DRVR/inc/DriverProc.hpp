#ifndef DRVR_INC_DRIVER_PROC
#define DRVR_INC_DRIVER_PROC

#include "DRVR/DriverProcServer.hpp"

#include <atomic>
#include <thread>
#include <fstream>

namespace DRVR
{

class DriverProc : public DriverProcServer
{
public:
    DriverProc(double position)
    :   DriverProcServer(),
        m_position(position),
        m_logFile( std::string("driver") + std::to_string(time(NULL)) + ".txt" )
    { }

    double position() override;
    double velocity() override;
    void loggingOn() override;
    void loggingOff() override;
    void run(int refreshRate_ms) override;
    void terminate() override;
    void setVelocity(double velocity) override;
    void setAcceleration(double acceleration) override;
    ~DriverProc() override = default;

private:
    using DriverProcServer::DriverProcServer;

    std::atomic<double> m_position{};
    std::atomic<double> m_velocity{};
    std::atomic<double> m_acceleration{};

    std::atomic<bool> m_logging{};
    std::ofstream m_logFile;
    long m_logCount{};

    std::atomic<bool> m_running{};
    std::thread m_background;
};

} // DRVR


#endif // DRVR_INC_DRIVER_PROC
