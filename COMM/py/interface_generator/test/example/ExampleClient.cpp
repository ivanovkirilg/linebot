// client-side GENERATED file

#include "ExampleClient.hpp"

#include "COMM/Socket.hpp"

#include "zpp_bits.h"


XMPL::ExampleClient::ExampleClient(const std::string& localAddress)
    : m_serverConnection(COMM::Socket::connect(0))
{
    throw std::runtime_error("Local address registry not implemented");
}

XMPL::ExampleClient::ExampleClient(int port)
    : m_serverConnection(COMM::Socket::connect(port))
{
}

XMPL::ExampleClient::~ExampleClient()
{
    std::lock_guard lock{m_mutex};

    constexpr int methodCode = -1;

    auto [inargs, write] = zpp::bits::data_out(zpp::bits::endian::network{});
    write(methodCode).or_throw();

    m_serverConnection.send(inargs);
}

void XMPL::ExampleClient::set(double speed)
{
    std::lock_guard lock{m_mutex};

    constexpr int methodCode = 0;

    auto [inargs, write] = zpp::bits::data_out(zpp::bits::endian::network{});
    write(methodCode).or_throw();
    write(speed).or_throw();

    m_serverConnection.send(inargs);
    auto response = m_serverConnection.receive();

    auto read = zpp::bits::in(response, zpp::bits::endian::network{});
    int errCode = 0;
    read(errCode).or_throw();
    if (errCode)
    {
        // TODO read message
        throw std::runtime_error("set failed");
    }

}

void XMPL::ExampleClient::get(double& speed)
{
    std::lock_guard lock{m_mutex};

    constexpr int methodCode = 1;

    auto [inargs, write] = zpp::bits::data_out(zpp::bits::endian::network{});
    write(methodCode).or_throw();


    m_serverConnection.send(inargs);
    auto response = m_serverConnection.receive();

    auto read = zpp::bits::in(response, zpp::bits::endian::network{});
    int errCode = 0;
    read(errCode).or_throw();
    if (errCode)
    {
        // TODO read message
        throw std::runtime_error("get failed");
    }
    read(speed).or_throw();
}

