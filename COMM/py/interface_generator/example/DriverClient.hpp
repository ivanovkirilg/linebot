// client-side GENERATED file

#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT

#include "COMM/Connection.hpp"

#include <string>


class DriverClient
{
public:
    DriverClient(const std::string& localAddress);
    DriverClient(int serverPort);

    void set(double speed);
    void get(double& speed);

private:
    COMM::Connection m_serverConnection;
};

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT
