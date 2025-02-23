#include "COMM/Connection.hpp"

#include "LOGR/Warning.hpp"

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <string>
#include <vector>

using namespace COMM;


namespace
{

inline std::string getUnderlyingError()
{
    auto e = errno;
    return std::to_string(e) + ": " + ::strerror(e);
}

} // namespace


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
            LOGR::Warning{getUnderlyingError()};
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
    if (m_fileDescriptor == 0)
    {
        throw LOGR::Exception("This Connection is already closed");
    }

    ssize_t sent = ::send(m_fileDescriptor, message.data(), message.size(), 0);

    if (sent < 0)
    {
        throw NetworkException(getUnderlyingError());
    }
    else if (sent == 0)
    {
        m_fileDescriptor = 0;
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
    if (m_fileDescriptor == 0)
    {
        throw LOGR::Exception("This Connection is already closed");
    }

    constexpr size_t CHUNK = 2048;
    std::array<std::byte, CHUNK> buffer{};

    ssize_t received = ::recv(m_fileDescriptor, buffer.data(), buffer.size(), 0);

    if (received < 0)
    {
        throw NetworkException(getUnderlyingError());
    }
    else if (received == 0)
    {
        m_fileDescriptor = 0;
        throw ConnectionClosedException("recv() Connection closed");
    }
    else if (received == CHUNK)
    {
        throw LOGR::Exception("Longer messages not supported");
    }

    std::vector<std::byte> message(buffer.begin(), buffer.begin() + received);

    return message;
}
