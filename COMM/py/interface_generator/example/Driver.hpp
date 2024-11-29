// server-side hand-coded file

#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER

#include "DriverServer.hpp"


class Driver : public DriverServer
{
public:
    using DriverServer::DriverServer;

    virtual void set(double speed) override;
    virtual void get(double& speed) override;

private:
    double m_speed = 0.0;
};


#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER
