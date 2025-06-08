#ifndef DRVR_INCLUDE_DRVR_DRIVER
#define DRVR_INCLUDE_DRVR_DRIVER

#include "IDriver.hpp"
#include "DRVR/DriverProcClient.hpp"

#include <chrono>

class Driver : public IDriver
{
public:
    Driver(int serverPort)
    :   m_client(serverPort)
    { }

    virtual double position() override;
    virtual double velocity() override;

    virtual void loggingOn() override;
    virtual void loggingOff() override;

    virtual void run(std::chrono::milliseconds refreshRate) override;
    virtual void terminate() override;

    virtual void setVelocity(double velocity) override;
    virtual void setAcceleration(double acceleration) override;

    virtual ~Driver() override = default;

private:
    DRVR::DriverProcClient m_client;
};

#endif // SRC_WAFER_STAGE_DRIVER
