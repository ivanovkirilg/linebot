#include "COMM/Socket.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace COMM;

Socket::Socket(int port)
{
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    const std::string portStr = std::to_string(port);

    addrinfo* socketAddress = nullptr;
    int result = ::getaddrinfo(nullptr, portStr.c_str(), &hints, &socketAddress);

    if (result)
    {
        std::cerr << "getaddrinfo() ERROR " << result
                  << ": " << ::gai_strerror(result) << std::endl;
        throw std::runtime_error("getaddrinfo ERROR");
    }

    m_addrInfo = socketAddress;


    m_fileDescriptor = ::socket(socketAddress->ai_family,
                                socketAddress->ai_socktype,
                                socketAddress->ai_protocol);

    if (m_fileDescriptor < 0)
    {
        std::cerr << "socket() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("socket ERROR");
    }
}

Socket::~Socket()
{
    addrinfo* socketAddress = (addrinfo*) m_addrInfo;

    if (socketAddress != nullptr)
    {
        ::freeaddrinfo(socketAddress);
    }

    if (m_fileDescriptor > 0)
    {
        int result = ::close(m_fileDescriptor);

        if (result < 0)
        {
            std::cerr << "close() ERROR " << errno
                      << ": " << ::strerror(errno) << std::endl;
        }
    }
}

void Socket::bind()
{
    addrinfo* socketAddress = (addrinfo*) m_addrInfo;
    int result = ::bind(m_fileDescriptor,
                        socketAddress->ai_addr,
                        socketAddress->ai_addrlen);

    if (result < 0)
    {
        std::cerr << "bind() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("bind ERROR");
    }
}

void Socket::listen(int backlog)
{
    int result = ::listen(m_fileDescriptor, backlog);

    if (result < 0)
    {
        std::cerr << "listen() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("listen ERROR");
    }
}

Connection Socket::accept()
{
    addrinfo clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int result = ::accept(m_fileDescriptor,
                          (sockaddr*) &clientAddr,
                          &clientAddrSize);

    if (result < 0)
    {
        std::cerr << "accept() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("accept ERROR");
    }

    return result;
}

Connection Socket::connect(int port)
{
    Socket temp(port);
    addrinfo* socketAddress = (addrinfo*) temp.m_addrInfo;

    int result = ::connect(temp.m_fileDescriptor,
                           socketAddress->ai_addr,
                           socketAddress->ai_addrlen);

    if (result < 0)
    {
        std::cout << "connect() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("connect ERROR");
    }

    Connection connection(temp.m_fileDescriptor);
    temp.m_fileDescriptor = 0;

    return connection;
}

Connection::Connection(int fileDescriptor)
    : m_fileDescriptor(fileDescriptor)
{
}

Connection::~Connection()
{
    int result = ::close(m_fileDescriptor);

    if (result < 0)
    {
        std::cerr << "close() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
    }

    m_fileDescriptor = 0;
}

Connection::operator bool()
{
    return m_fileDescriptor != 0;
}

void Connection::send(std::string message)
{
    int sent = ::send(m_fileDescriptor, message.data(), message.size(), 0);

    if (sent < 0)
    {
        std::cout << "send() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("send() ERROR");
    }
    else if (sent == 0)
    {
        std::cout << "Connection closed" << std::endl;
        throw std::runtime_error("send() Connection closed");
    }
    else if (sent < message.size())
    {
        std::cout << "TODO message partially sent" << std::endl;
        throw std::runtime_error("TODO send rest of message");
    }
}

std::string Connection::receive()
{
    constexpr const size_t CHUNK = 1024;
    char buffer[CHUNK];

    ssize_t received = ::recv(m_fileDescriptor, buffer, CHUNK, 0);

    if (received < 0)
    {
        std::cout << "recv() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("recv() ERROR");
    }
    else if (received == 0)
    {
        std::cout << "Connection closed" << std::endl;
        throw std::runtime_error("recv() Connection closed");
    }
    else if (received == CHUNK)
    {
        std::cout << "TODO message partially received" << std::endl;
        throw std::runtime_error("TODO receive rest of message");
    }

    buffer[received] = '\0';

    return std::string(buffer);
}


