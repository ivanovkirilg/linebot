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

    virtual void accelerate(double instantaneousAcceleration) = 0;

    virtual ~IDriver() = default;
};

#endif // DRVR_INCLUDE_DRVR_IDRIVER
