// server-side GENERATED file

#ifndef COMM_PY_INTERFACE_GENERATOR_TEST_EXAMPLE_EXAMPLE_SERVER
#define COMM_PY_INTERFACE_GENERATOR_TEST_EXAMPLE_EXAMPLE_SERVER

#include "COMM/Connection.hpp"
#include "COMM/Server.hpp"


namespace XMPL
{

class ExampleServer : public COMM::Server
{
public:
    using Server::Server;

protected:
    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

private:
    void handleRequest(COMM::Connection& client) override;
};

} // XMPL

#endif // COMM_PY_INTERFACE_GENERATOR_TEST_EXAMPLE_EXAMPLE_SERVER
