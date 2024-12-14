// server-side GENERATED file

#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER

#include "COMM/Connection.hpp"
#include "COMM/Server.hpp"


namespace DRVR
{

class DriverServer : public COMM::Server
{
public:
    using Server::Server;

protected:
    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

private:
    void handleRequest(COMM::Connection& client) override;
};

} // DRVR

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER
