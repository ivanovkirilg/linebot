#include "COMM/Connection.hpp"

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace COMM;

Connection::Connection(int fileDescriptor)
    : m_fileDescriptor(fileDescriptor)
{
}

Connection::~Connection()
{
    if (m_fileDescriptor > 0)
    {
        int result = ::close(m_fileDescriptor);

        if (result < 0)
        {
            std::cerr << "close() ERROR " << errno
                      << ": " << ::strerror(errno) << std::endl;
        }

        m_fileDescriptor = 0;
    }
}

Connection::Connection(Connection&& other)
{
    *this = std::move(other);
}

Connection& Connection::operator=(Connection&& other)
{
    m_fileDescriptor = other.m_fileDescriptor;
    other.m_fileDescriptor = 0;
    return *this;
}

Connection::operator bool()
{
    return m_fileDescriptor != 0;
}

int Connection::fileDescriptor()
{
    return m_fileDescriptor;
}

void Connection::send(std::vector<std::byte> message)
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
        throw COMM::ConnectionClosedException("send() Connection closed");
    }
    else if (sent < message.size())
    {
        std::cout << "TODO message partially sent" << std::endl;
        throw std::runtime_error("TODO send rest of message");
    }
}

std::vector<std::byte> Connection::receive()
{
    constexpr const size_t CHUNK = 1024;
    std::array<std::byte, CHUNK> buffer;

    ssize_t received = ::recv(m_fileDescriptor, buffer.data(), buffer.size(), 0);

    if (received < 0)
    {
        std::cout << "recv() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("recv() ERROR");
    }
    else if (received == 0)
    {
        std::cout << "Connection closed" << std::endl;
        throw COMM::ConnectionClosedException("recv() Connection closed");
    }
    else if (received == CHUNK)
    {
        std::cout << "TODO message partially received" << std::endl;
        throw std::runtime_error("TODO receive rest of message");
    }

    std::vector<std::byte> message(received);

    std::copy(buffer.begin(), buffer.begin() + received, message.begin());

    return message;
}
