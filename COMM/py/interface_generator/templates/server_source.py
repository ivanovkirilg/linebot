
PARAMS_DECL_TEMPLATE = '                {type} {name}{{}};'

IN_PARAMS_TEMPLATE = '                in({name}).or_throw();'

OUT_PARAMS_TEMPLATE = '                out({name}).or_throw();'

METHOD_TEMPLATE = """
            case {index}:
            {{
{params_declaration}
{read_in_params}
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
{write_out_params}
                break;
            }}
"""

FILE_TEMPLATE = """// server-side GENERATED file

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

ConnectionStatus {namespace}::{interface}Server::handleRequest(Connection& client)
{{
    const std::vector<std::byte> inBuffer = client.receive();
    auto in = zpp::bits::in(inBuffer, zpp::bits::endian::network{{}});
    auto [reply, out] = zpp::bits::data_out(zpp::bits::endian::network{{}});

    int methodCode = 0;
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
            case -1:
            {{
                return ConnectionStatus::DISCONNECTED;
                break;
            }}
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
    return ConnectionStatus::CONNECTED;
}}
"""
