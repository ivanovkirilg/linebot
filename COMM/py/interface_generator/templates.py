
CLIENT_HEADER_FORMAT = """
#include "COMM/Connection.hpp"

#include <string>


namespace {namespace}
{{

class {interface}Client
{{
public:
    {interface}Client(const std::string& localAddress);
    {interface}Client(int serverPort);

{methods}
private:
    COMM::Connection m_serverConnection;
}};

}} // {namespace}
"""

SERVER_HEADER_FORMAT = """
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
    void handleRequest(COMM::Connection& client) override;
}};

}} // {namespace}
"""

CLIENT_METHOD_FORMAT = "    {ret} {name}({params});"

SERVER_METHOD_FORMAT = "    virtual {ret} {name}({params}) = 0;"


CLIENT_SOURCE_FORMAT = """
#include "{header}"

#include "COMM/Socket.hpp"

#include "zpp_bits.hpp"


{namespace}::{interface}Client::{interface}Client(const std::string& localAddress)
    : m_serverConnection(COMM::Socket::connect(0))
{{
    throw std::runtime_error("Local address registry not implemented");
}}

{namespace}::{interface}Client::{interface}Client(int port)
    : m_serverConnection(COMM::Socket::connect(port))
{{
}}

{methods}
"""

CLIENT_SOURCE_METHOD_FORMAT = """
{ret} {namespace}::{interface}Client::{name}({params})
{{
    constexpr int methodCode = {index};

    auto [inargs, write] = zpp::bits::data_out(zpp::bits::endian::network{{}});
    write(methodCode).or_throw();
{input_params_serialization}

    m_serverConnection.send(inargs);
    auto response = m_serverConnection.receive();

    auto read = zpp::bits::in(response, zpp::bits::endian::network{{}});
    int errCode = 0;
    read(errCode).or_throw();
    if (errCode)
    {{
        // TODO read message
        throw std::runtime_error("{name} failed");
    }}
{output_params_deserialization}
}}
"""

CLIENT_SERIALIZE_INPUT_FORMAT = '    write({name}).or_throw();'

CLIENT_DESERIALIZE_OUTPUT_FORMAT = '    read({name}).or_throw();'
