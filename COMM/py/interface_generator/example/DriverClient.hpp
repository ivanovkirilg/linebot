// client-side GENERATED file

#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT

#include "COMM/Socket.hpp"


class DriverClient
{
public:
    DriverClient(int serverPort);

    void set(double speed);
    void get(double& speed);

private:
    COMM::Connection m_serverConnection;
};

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT
