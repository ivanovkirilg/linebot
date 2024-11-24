// server-side GENERATED file

#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER

#include "COMM/Socket.hpp"


class DriverServer
{
public:
    void handleRequest(COMM::Connection& client);

protected:
    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

private:
};

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER
