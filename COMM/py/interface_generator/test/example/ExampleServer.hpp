// server-side GENERATED file

#ifndef _TMP_EXAMPLE_SERVER
#define _TMP_EXAMPLE_SERVER

#include "COMM/Connection.hpp"
#include "COMM/Server.hpp"


namespace XMPL
{

class ExampleServer : public COMM::Server
{
public:
    using Server::Server;

protected:
    virtual void set(double value) = 0;
    virtual double get() = 0;
    virtual void calculate(double& square, double& cube) = 0;

private:
    COMM::Server::ConnectionStatus handleRequest(COMM::Connection& client) override;
};

} // XMPL

#endif // _TMP_EXAMPLE_SERVER
