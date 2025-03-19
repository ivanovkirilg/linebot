
IN_PARAMS_TEMPLATE = '    write({name}).or_throw();'

OUT_PARAMS_TEMPLATE = '    read({name}).or_throw();'

METHOD_TEMPLATE = """
{ret} {namespace}::{interface}Client::{name}({params})
{{
    std::lock_guard lock{{m_mutex}};

    constexpr int methodCode = {index};

    auto [inargs, write] = zpp::bits::data_out(zpp::bits::endian::network{{}});
    write(methodCode).or_throw();
{serialize_in_params}

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
{deserialize_out_params}
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
    std::lock_guard lock{{m_mutex}};

    constexpr int methodCode = -1;

    auto [inargs, write] = zpp::bits::data_out(zpp::bits::endian::network{{}});
    write(methodCode).or_throw();

    m_serverConnection.send(inargs);
}}
{methods}
"""
