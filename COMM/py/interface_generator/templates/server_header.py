
METHOD_TEMPLATE = "    virtual {ret} {name}({params}) = 0;"

FILE_TEMPLATE = """// server-side GENERATED file

{header_guard}

#include "COMM/Connection.hpp"
#include "COMM/Server.hpp"


namespace {namespace}
{{

class {interface}Server : public COMM::Server
{{
public:
    using Server::Server;

protected:
{methods}
private:
    COMM::Server::ConnectionStatus handleRequest(COMM::Connection& client) override;
}};

}} // {namespace}

{end_header_guard}
"""
