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
    std::lock_guard COMM_lock{m_mutex};

    constexpr int COMM_methodCode = -1;

    auto [COMM_inargs, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{});
    COMM_write(COMM_methodCode).or_throw();

    m_serverConnection.send(COMM_inargs);
}

void XMPL::ExampleClient::set(double value)
{
    std::lock_guard COMM_lock{m_mutex};

    constexpr int COMM_methodCode = 1;
    // no return

    auto [COMM_inargs, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{});
    COMM_write(COMM_methodCode).or_throw();
    COMM_write(value).or_throw();

    m_serverConnection.send(COMM_inargs);
    auto COMM_response = m_serverConnection.receive();

    auto COMM_read = zpp::bits::in(COMM_response, zpp::bits::endian::network{});
    int COMM_errCode = 0;
    COMM_read(COMM_errCode).or_throw();
    if (COMM_errCode)
    {
        // TODO read message
        throw std::runtime_error("set failed");
    }

    // no return
}

double XMPL::ExampleClient::get()
{
    std::lock_guard COMM_lock{m_mutex};

    constexpr int COMM_methodCode = 2;
    double COMM_retval{};

    auto [COMM_inargs, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{});
    COMM_write(COMM_methodCode).or_throw();


    m_serverConnection.send(COMM_inargs);
    auto COMM_response = m_serverConnection.receive();

    auto COMM_read = zpp::bits::in(COMM_response, zpp::bits::endian::network{});
    int COMM_errCode = 0;
    COMM_read(COMM_errCode).or_throw();
    if (COMM_errCode)
    {
        // TODO read message
        throw std::runtime_error("get failed");
    }

    COMM_read(COMM_retval).or_throw();
    return COMM_retval;
}

void XMPL::ExampleClient::calculate(double& square, double& cube)
{
    std::lock_guard COMM_lock{m_mutex};

    constexpr int COMM_methodCode = 3;
    // no return

    auto [COMM_inargs, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{});
    COMM_write(COMM_methodCode).or_throw();


    m_serverConnection.send(COMM_inargs);
    auto COMM_response = m_serverConnection.receive();

    auto COMM_read = zpp::bits::in(COMM_response, zpp::bits::endian::network{});
    int COMM_errCode = 0;
    COMM_read(COMM_errCode).or_throw();
    if (COMM_errCode)
    {
        // TODO read message
        throw std::runtime_error("calculate failed");
    }
    COMM_read(square).or_throw();
    COMM_read(cube).or_throw();
    // no return
}

