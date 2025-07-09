
PARAMS_DECL_TEMPLATE = '                {type} {name}{{}};'

IN_PARAMS_TEMPLATE = '                COMM_read({name}).or_throw();'

OUT_PARAMS_TEMPLATE = '                COMM_write({name}).or_throw();'

RET_TEMPORARY_TEMPLATE = '{ret} COMM_retval{{}};' # or '' if void

RET_ASSIGNMENT_TEMPLATE = 'COMM_retval = ' # or '' if void

RET_WRITE_TEMPLATE = 'COMM_write(COMM_retval).or_throw();' # or '' if void

METHOD_TEMPLATE = """
            case {index}:
            {{
                {ret_temporary}
{params_declaration}
{read_in_params}
                try
                {{
                    {ret_assignment}{name}({params});
                }}
                catch (LOGR::Exception& exc)
                {{
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("{name}", exc))).or_throw();
                    exc.handle(Messages::SEND_ERR_TO_CLIENT);
                }}
                catch (std::exception& exc)
                {{
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("{name}", exc))).or_throw();
                }}
                COMM_write(RequestResult::OK).or_throw();
{write_out_params}
                {ret_write}
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

Server::ConnectionStatus {namespace}::{interface}Server::handleRequest(Connection& client)
{{
    const std::vector<std::byte> COMM_inargs = client.receive();
    auto COMM_read = zpp::bits::in(COMM_inargs, zpp::bits::endian::network{{}});
    auto [COMM_reply, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{{}});

    int COMM_methodCode = 0;
    if (zpp::bits::failure(COMM_read(COMM_methodCode))) // Read fail is probably not our fault
    {{
        LOGR::Warning{{Messages::COULD_NOT_READ_METHOD_CODE}};

        COMM_write(ErrorResult(RequestResult::PROTOCOL_ERROR,
                   Messages::COULD_NOT_READ_METHOD_CODE)).or_throw(); // Write fail shoudn't happen
    }}
    else
    {{
        switch (COMM_methodCode)
        {{
            case -1:
            {{
                return Server::ConnectionStatus::DISCONNECTED;
                break;
            }}
{methods}
            default:
            {{
                COMM_write(ErrorResult(RequestResult::PROTOCOL_ERROR,
                           Messages::BAD_METHOD_CODE
                           + std::to_string(COMM_methodCode))).or_throw();
            }}
        }}

        const size_t COMM_leftover = COMM_read.remaining_data().size();
        if (COMM_leftover)
        {{
            COMM_reply.clear();
            COMM_write.reset();
            COMM_write(RequestResult::PROTOCOL_ERROR).or_throw();
            COMM_write(Messages::LEFTOVER_DATA + std::to_string(COMM_leftover)).or_throw();
        }}
    }}

    client.send(COMM_reply);
    return Server::ConnectionStatus::CONNECTED;
}}
"""
