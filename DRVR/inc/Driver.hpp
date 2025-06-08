#ifndef DRVR_INC_DRIVER_PROC
#define DRVR_INC_DRIVER_PROC

#include "DRVR/DriverServer.hpp"

#include <atomic>
#include <thread>
#include <fstream>

namespace DRVR
{

///
/// The `linebot` positioning driver, which asynchronously simulates movement
/// according to the set velocity or acceleration.
///
class Driver : public DriverServer
{
public:
    Driver(double position)
    :   DriverServer(),
        m_position(position),
        m_logFile( std::string("driver") + std::to_string(time(NULL)) + ".txt" )
    { }

    /// Current position in the range [0, 1] units.
    double position() override;

    /// Current velocity in units/s.
    double velocity() override;

    /// \brief Start a new log line in the current log-file,
    /// containing tab-delimited positions at each refresh/update.
    void loggingOn() override;

    /// \brief End the current log line.
    /// \pre `loggingOn()` has been called.
    void loggingOff() override;

    /// \brief Start the background simulation, updating at `refreshRate`.
    void run(int refreshRate_ms) override;

    /// \brief Stop the simulation at the next update.
    /// \note It can be re-`run()` again.
    void terminate() override;

    /// \brief Set a *constant* velocity vector (in units/s).
    /// \note Nullifies acceleration, if any.
    void setVelocity(double velocity) override;

    /// \brief Set an acceleration/deceleration vector (in units/s^2).
    /// \note When decelerating, the vector will be maintained after reaching
    /// 0 speed, resulting in acceleration.
    void setAcceleration(double acceleration) override;

    ~Driver() override = default;

private:
    using DriverServer::DriverServer;

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
