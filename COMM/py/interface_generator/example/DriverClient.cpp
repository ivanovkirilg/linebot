// client-side GENERATED file

#include "DriverClient.hpp"

#include "zpp_bits.hpp"

DriverClient::DriverClient(int port)
    : m_serverConnection(COMM::Socket::connect(port))
{
}

void DriverClient::set(double speed)
{
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
        throw "set failed";
    }
}

void DriverClient::get(double& speed)
{
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
        throw "set failed";
    }

    read(speed).or_throw();
}
