#include "COMM/Watcher.hpp"

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <stdexcept>
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
    if (m_epollFileDescriptor > 0)
    {
        int result = ::close(m_epollFileDescriptor);

        if (result != 0)
        {
            std::cout << "close() ERROR " << errno
                      << ": " << ::strerror(errno) << std::endl;
        }
    }

    for (auto& watched : m_watched)
    {
        int result = ::close(watched.second->fileDescriptor());

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
                             event.data.fd, &event);

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
    epoll_event event = {0};

    int result = ::epoll_ctl(m_epollFileDescriptor, EPOLL_CTL_DEL,
                             watchable->fileDescriptor(), &event);

    if (result != 0)
    {
        std::cout << "epoll_ctl() ERROR " << errno
                  << ": " << ::strerror(errno) << std::endl;
        throw std::runtime_error("epoll_ctl() ERROR");
    }

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
