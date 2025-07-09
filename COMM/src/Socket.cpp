#include "COMM/Socket.hpp"

#include "COMM/Connection.hpp"

#include "LOGR/Exception.hpp"
#include "LOGR/Warning.hpp"

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

#include <cassert>
#include <cstring>

using namespace COMM;

namespace
{

int getPort(int socketFileDescriptor)
{
    sockaddr_in actualAddr{};
    socklen_t size = sizeof(actualAddr);
    int result = ::getsockname(socketFileDescriptor,
                               (sockaddr*) &actualAddr, &size);

    if (result < 0)
    {
        throw NetworkException("getsockname ERROR " + LOGR::getUnderlyingError());
    }

    return actualAddr.sin_port;
}

}

Socket::Socket(int port)
    : m_port(port)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    const std::string portStr = std::to_string(port);

    addrinfo* socketAddress = nullptr;
    int result = ::getaddrinfo(nullptr, portStr.c_str(), &hints, &socketAddress);

    if (result != 0)
    {
        throw NetworkException("getaddrinfo ERROR " + LOGR::getUnderlyingError());
    }

    m_addrInfo = socketAddress;


    m_fileDescriptor = ::socket(socketAddress->ai_family,
                                socketAddress->ai_socktype,
                                socketAddress->ai_protocol);

    if (m_fileDescriptor < 0)
    {
        throw NetworkException("socket ERROR " + LOGR::getUnderlyingError());
    }
}

Socket::~Socket()
{
    addrinfo* socketAddress = (addrinfo*) m_addrInfo;

    if (socketAddress != nullptr)
    {
        ::freeaddrinfo(socketAddress);
    }

    if (m_fileDescriptor >= 0)
    {
        int result = ::close(m_fileDescriptor);

        if (result < 0)
        {
            LOGR::Warning{LOGR::getUnderlyingError()};
        }
    }
}

int Socket::port() const
{
    return m_port;
}

int Socket::fileDescriptor() const
{
    return m_fileDescriptor;
}

void Socket::bind()
{
    if (m_addrInfo == nullptr) throw std::logic_error{"m_addrInfo == nullptr"};

    addrinfo* socketAddress = (addrinfo*) m_addrInfo;
    int result = ::bind(m_fileDescriptor,
                        socketAddress->ai_addr,
                        socketAddress->ai_addrlen);

    if (result < 0)
    {
        throw NetworkException("bind ERROR " + LOGR::getUnderlyingError());
    }

    if (m_port == 0)
    {
        m_port = getPort(m_fileDescriptor);
    }
}

void Socket::listen(int backlog)
{
    int result = ::listen(m_fileDescriptor, backlog);

    if (result < 0)
    {
        throw NetworkException("listen ERROR " + LOGR::getUnderlyingError());
    }
}

Connection Socket::accept()
{
    addrinfo clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);
    int result = ::accept(m_fileDescriptor,
                          (sockaddr*) &clientAddr,
                          &clientAddrSize);

    if (result < 0)
    {
        throw NetworkException("accept ERROR " + LOGR::getUnderlyingError());
    }

    return {result};
}

Connection Socket::connect(int port)
{
    Socket temp(port); // TODO better without this?
    addrinfo* socketAddress = (addrinfo*) temp.m_addrInfo;

    int result = ::connect(temp.m_fileDescriptor,
                           socketAddress->ai_addr,
                           socketAddress->ai_addrlen);

    if (result < 0)
    {
        throw NetworkException("connect ERROR " + LOGR::getUnderlyingError());
    }

    Connection connection(temp.m_fileDescriptor);
    temp.m_fileDescriptor = -1;

    return connection;
}
