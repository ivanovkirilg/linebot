#include "COMM/Connection.hpp"

#include "LOGR/Warning.hpp"
#include "LOGR/Exception.hpp"

#include <cassert>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <string>
#include <vector>

using namespace COMM;


Connection::Connection(int fileDescriptor)
    : m_fileDescriptor(fileDescriptor),
      m_open(fileDescriptor >= 0)
{
    assert(fileDescriptor >= 0);
}

Connection::~Connection()
{
    if ((m_fileDescriptor >= 0) && m_open)
    {
        int result = ::close(m_fileDescriptor);

        if (result < 0)
        {
            LOGR::Warning{LOGR::getUnderlyingError()};
        }

        m_open = false;
    }
}

Connection::Connection(Connection&& other)
{
    *this = std::move(other);
}

Connection& Connection::operator=(Connection&& other)
{
    m_open = other.m_open;
    other.m_open = false;
    m_fileDescriptor = other.m_fileDescriptor;
    return *this;
}

Connection::operator bool() const
{
    return m_open;
}

int Connection::fileDescriptor() const
{
    return m_fileDescriptor;
}

void Connection::send(std::vector<std::byte> message)
{
    if (not m_open)
    {
        throw ConnectionClosedException("This Connection is already closed");
    }

    ssize_t sent = ::send(m_fileDescriptor, message.data(), message.size(), 0);

    if (sent < 0)
    {
        throw NetworkException(LOGR::getUnderlyingError());
    }
    else if (sent == 0)
    {
        m_open = false;
        throw ConnectionClosedException("send() Connection closed");
    }
    else if (sent < (ssize_t) message.size())
    {
        // Apparently this case is possible per documentation,
        // but not with common implementations???
        LOGR::Warning{"Sent less than expected:", sent, "<", message.size()};
        send( { message.begin() + sent, message.end() } );
    }
}

std::vector<std::byte> Connection::receive()
{
    if (not m_open)
    {
        throw ConnectionClosedException("This Connection is already closed");
    }

    constexpr size_t CHUNK = 4096;
    std::array<std::byte, CHUNK> buffer{};

    ssize_t received = ::recv(m_fileDescriptor, buffer.data(), buffer.size(), 0);

    if (received < 0)
    {
        throw NetworkException(LOGR::getUnderlyingError());
    }
    else if (received == 0)
    {
        m_open = false;
        throw ConnectionClosedException("recv() Connection closed");
    }
    else if (received == CHUNK)
    {
        // FIXME we should keep recv'ing
        throw LOGR::Exception("Longer messages not supported");
    }

    std::vector<std::byte> message(buffer.begin(), buffer.begin() + received);

    return message;
}
