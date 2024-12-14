// client-side GENERATED file

#include "DriverClient.hpp"

#include "COMM/Socket.hpp"

#include "zpp_bits.hpp"


DRVR::DriverClient::DriverClient(const std::string& localAddress)
    : m_serverConnection(COMM::Socket::connect(0))
{
    throw std::runtime_error("Local address registry not implemented");
}

DRVR::DriverClient::DriverClient(int port)
    : m_serverConnection(COMM::Socket::connect(port))
{
}


void DRVR::DriverClient::set(double speed)
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
        // TODO read message
        throw std::runtime_error("set failed");
    }

}

void DRVR::DriverClient::get(double& speed)
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
        // TODO read message
        throw std::runtime_error("get failed");
    }
    read(speed).or_throw();
}
