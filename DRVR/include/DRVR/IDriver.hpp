#ifndef DRVR_INCLUDE_DRVR_IDRIVER
#define DRVR_INCLUDE_DRVR_IDRIVER

#include <chrono>


class IDriver
{
public:
    virtual double position() const = 0;
    virtual double velocity() const = 0;

    virtual void loggingOn() = 0;
    virtual void loggingOff() = 0;

    virtual void run(std::chrono::milliseconds refreshRate) = 0;
    virtual void terminate() = 0;

    virtual void setVelocity(double velocity) = 0;
    virtual void setAcceleration(double acceleration) = 0;
};

#endif // DRVR_INCLUDE_DRVR_IDRIVER
