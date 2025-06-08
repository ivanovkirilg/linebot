#ifndef DRVR_INCLUDE_DRVR_IDRIVER
#define DRVR_INCLUDE_DRVR_IDRIVER

#include <chrono>


///
/// The `linebot` positioning driver, which asynchronously simulates movement
/// according to the set velocity or acceleration.
///
class IDriver
{
public:
    /// Current position in the range [0, 1] units.
    virtual double position() = 0;
    /// Current velocity in units/s.
    virtual double velocity() = 0;

    /// \brief Start a new log line in the current log-file,
    /// containing tab-delimited positions at each refresh/update.
    virtual void loggingOn() = 0;

    /// \brief End the current log line.
    /// \pre `loggingOn()` has been called.
    virtual void loggingOff() = 0;

    /// \brief Start the background simulation, updating at `refreshRate`.
    virtual void run(std::chrono::milliseconds refreshRate) = 0;
    /// \brief Stop the simulation at the next update.
    /// \note It can be re-`run()` again.
    virtual void terminate() = 0;

    /// \brief Set a *constant* velocity vector (in units/s).
    /// \note Nullifies acceleration, if any.
    virtual void setVelocity(double velocity) = 0;

    /// \brief Set an acceleration/deceleration vector (in units/s^2).
    /// \note When decelerating, the vector will be maintained,
    /// resulting in acceleration.
    virtual void setAcceleration(double acceleration) = 0;

    virtual ~IDriver() = default;
};

#endif // DRVR_INCLUDE_DRVR_IDRIVER
