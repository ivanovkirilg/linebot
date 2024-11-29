// server-side GENERATED file

#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER

#include "COMM/Socket.hpp"

#include "Server.hpp"


class DriverServer : public Server
{
public:
    using Server::Server;

    // TODO Extract to class or interface `Server`?
    void handleRequest(COMM::Connection& client) override;

protected:
    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

private:
};

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_SERVER
