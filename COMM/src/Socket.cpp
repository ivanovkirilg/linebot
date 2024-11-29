#include "COMM/Socket.hpp"

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

namespace
{

int getPort(int socketFileDescriptor)
{
    sockaddr_in actualAddr = {0};
    socklen_t size = sizeof(actualAddr);
    int result = ::getsockname(socketFileDescriptor,
                               (sockaddr*) &actualAddr, &size);

    if (result < 0)
    {
        std::cerr << "getsockname() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("getsockname ERROR");
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

int Socket::port() const
{
    return m_port;
}

int Socket::fileDescriptor()
{
    return m_fileDescriptor;
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

Watcher::Watcher()
{
    m_epollFileDescriptor = ::epoll_create(1); // Must be > 0, but is ignored

    if (m_epollFileDescriptor < 0)
    {
        std::cout << "epoll_create() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("epoll_create() ERROR");
    }
}

Watcher::~Watcher()
{
    if (m_epollFileDescriptor >= 0) // TODO is this right?
    {
        int result = ::close(m_epollFileDescriptor);

        if (result != 0)
        {
            std::cout << "close() ERROR " << errno
                      << ": " << ::strerror(errno) << std::endl;
        }
    }
}

void Watcher::watch(std::shared_ptr<IWatchable> watchable)
{
    epoll_event event = {
        .events = EPOLLIN,
        .data = {
            .fd = watchable->fileDescriptor()
        }
    };

    int result = ::epoll_ctl(m_epollFileDescriptor, EPOLL_CTL_ADD,
                             watchable->fileDescriptor(), &event);

    if (result != 0)
    {
        std::cout << "epoll_ctl() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("epoll_ctl() ERROR");
    }
    m_watched.emplace(watchable->fileDescriptor(), watchable);
}

void Watcher::unwatch(std::shared_ptr<IWatchable> watchable)
{
    m_watched.erase(watchable->fileDescriptor());
}

std::vector<std::shared_ptr<IWatchable>> Watcher::wait(int msTimeout)
{
    constexpr size_t maxEvents = 10;
    epoll_event events[maxEvents] = {0};

    int result = ::epoll_wait(m_epollFileDescriptor, events, maxEvents, msTimeout);

    if (result < 0)
    {
        std::cout << "epoll_wait() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("epoll_wait() ERROR");
    }

    std::vector<std::shared_ptr<IWatchable>> ready;
    for (int i = 0; i < result; i++)
    {
        auto it = m_watched.find(events[i].data.fd);

        ready.push_back(it->second);
    }

    return ready;
}
