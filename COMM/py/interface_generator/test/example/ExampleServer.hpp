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
    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

private:
    COMM::ConnectionStatus handleRequest(COMM::Connection& client) override;
};

} // XMPL

#endif // _TMP_EXAMPLE_SERVER
