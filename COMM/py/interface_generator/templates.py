
CLIENT_HEADER_FORMAT = """
#include "COMM/Connection.hpp"

#include <string>


namespace {namespace}
{{

class I{interface}Client
{{
public:
    virtual ~I{interface}Client() = default;

{abstract_methods}
}};

class {interface}Client : public I{interface}Client
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

CLIENT_ABSTRACT_METHOD_FORMAT = "    virtual {ret} {name}({params}) = 0;"
CLIENT_METHOD_FORMAT          = "    {ret} {name}({params}) override;"

SERVER_METHOD_FORMAT = "    virtual {ret} {name}({params}) = 0;"


CLIENT_SOURCE_FORMAT = """
#include "{header}"

#include "COMM/Socket.hpp"

#include "zpp_bits.h"


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

SERVER_SOURCE_FORMAT = """
// server-side GENERATED file

#include "{interface}Server.hpp"

#include "COMM/InterfaceProtocol.hpp"

#include "LOGR/Exception.hpp"
#include "LOGR/Warning.hpp"
#include "LOGR/ILogger.hpp"

#include "zpp_bits.h"

#include <vector>
#include <sstream>

using namespace COMM;

namespace
{{

std::string buildFailureMessage(const char* func, const LOGR::Exception& exc)
{{
    std::ostringstream message;
    message << "{namespace}::{interface}Server::" << func
            << ": LOGR::Exception [" << exc.id() << "]: " << exc.what();
    return message.str();
}}

std::string buildFailureMessage(const char* func, const std::exception& exc)
{{
    std::ostringstream message;
    message << "{namespace}::{interface}Server::" << func
            << ": std::exception: " << exc.what();
    return message.str();
}}

struct ErrorResult
{{
    RequestResult code;
    std::string message;
}};

}}

void {namespace}::{interface}Server::handleRequest(Connection& client)
{{
    const std::vector<std::byte> inBuffer = client.receive();
    auto in = zpp::bits::in(inBuffer, zpp::bits::endian::network{{}});
    auto [reply, out] = zpp::bits::data_out(zpp::bits::endian::network{{}});

    int methodCode = -1;
    if (zpp::bits::failure(in(methodCode))) // Read fail is probably not our fault
    {{
        if (LOGR::ILogger::isSet())
        {{
            LOGR::Warning{{Messages::COULD_NOT_READ_METHOD_CODE}};
        }}

        out(ErrorResult(RequestResult::PROTOCOL_ERROR,
                        Messages::COULD_NOT_READ_METHOD_CODE)).or_throw(); // Write fail shoudn't happen
    }}
    else
    {{
        switch (methodCode)
        {{
{methods}
            default:
            {{
                out(ErrorResult(RequestResult::PROTOCOL_ERROR,
                                Messages::BAD_METHOD_CODE
                                + std::to_string(methodCode))).or_throw();
            }}
        }}

        const size_t leftover = in.remaining_data().size();
        if (leftover)
        {{
            reply.clear();
            out.reset();
            out(RequestResult::PROTOCOL_ERROR).or_throw();
            out(Messages::LEFTOVER_DATA + std::to_string(leftover)).or_throw();
        }}
    }}

    client.send(reply);
}}
"""

SERVER_SOURCE_METHOD_FORMAT = """
            case {index}:
            {{
{params_declaration}
{input_params_deserialization}
                try
                {{
                    {name}({params});
                }}
                catch (LOGR::Exception& exc)
                {{
                    out(ErrorResult(RequestResult::METHOD_FAILED,
                                    buildFailureMessage("{name}", exc))).or_throw();
                    exc.handle(Messages::SEND_ERR_TO_CLIENT);
                }}
                catch (std::exception& exc)
                {{
                    out(ErrorResult(RequestResult::METHOD_FAILED,
                                    buildFailureMessage("{name}", exc))).or_throw();
                }}
                out(RequestResult::OK).or_throw();
{output_params_serialization}
                break;
            }}
"""

SERVER_PARAMS_DECLARATION_FORMAT = '                {type} {name}{{}};'

SERVER_DESERIALIZE_INPUT_FORMAT = '                in({name}).or_throw();'

SERVER_SERIALIZE_OUTPUT_FORMAT = '                out({name}).or_throw();'
