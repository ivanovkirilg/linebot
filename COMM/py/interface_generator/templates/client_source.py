
IN_PARAMS_TEMPLATE = '    COMM_write({name}).or_throw();'

OUT_PARAMS_TEMPLATE = '    COMM_read({name}).or_throw();'

RET_TEMPORARY_TEMPLATE = '{ret} COMM_retval{{}};' # or '' if void

RETURN_TEMPLATE = '    COMM_read(COMM_retval).or_throw();' + \
                '\n    return COMM_retval;'           # or '' if void

METHOD_TEMPLATE = """
{ret} {namespace}::{interface}Client::{name}({params})
{{
    std::lock_guard COMM_lock{{m_mutex}};

    constexpr int COMM_methodCode = {index};
    {ret_temporary}

    auto [COMM_inargs, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{{}});
    COMM_write(COMM_methodCode).or_throw();
{write_in_params}

    m_serverConnection.send(COMM_inargs);
    auto COMM_response = m_serverConnection.receive();

    auto COMM_read = zpp::bits::in(COMM_response, zpp::bits::endian::network{{}});
    int COMM_errCode = 0;
    COMM_read(COMM_errCode).or_throw();
    if (COMM_errCode)
    {{
        // TODO read message
        throw std::runtime_error("{name} failed");
    }}
{read_out_params}
{read_and_return}
}}
"""

FILE_TEMPLATE = """// client-side GENERATED file

#include "{interface}Client.hpp"

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

{namespace}::{interface}Client::~{interface}Client()
{{
    std::lock_guard COMM_lock{{m_mutex}};

    constexpr int COMM_methodCode = -1;

    auto [COMM_inargs, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{{}});
    COMM_write(COMM_methodCode).or_throw();

    m_serverConnection.send(COMM_inargs);
}}
{methods}
"""
