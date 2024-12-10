
CLIENT_HEADER_FORMAT = """
#include "COMM/Connection.hpp"

#include <string>


class {interface}Client
{{
public:
    {interface}Client(const std::string& localAddress);
    {interface}Client(int serverPort);

{methods}
private:
    COMM::Connection m_serverConnection;
}};
"""

SERVER_HEADER_FORMAT = """
#include "COMM/Connection.hpp"
#include "COMM/Server.hpp"


class {interface}Server : public Server
{{
public:
    using Server::Server;

protected:
{methods}
private:
    void handleRequest(COMM::Connection& client) override;
}};
"""

CLIENT_METHOD_FORMAT = "    {ret} {name}({params});"

SERVER_METHOD_FORMAT = "    virtual {ret} {name}({params}) = 0;"
