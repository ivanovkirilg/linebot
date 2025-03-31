#include "Watcher.hpp"

#include "LOGR/Exception.hpp"
#include "LOGR/Warning.hpp"

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <vector>

using namespace COMM;


Watcher::Watcher()
{
    m_epollFileDescriptor = ::epoll_create(1); // Must be > 0, but is ignored

    if (m_epollFileDescriptor < 0)
    {
        throw PollingException("epoll_create() ERROR" + LOGR::getUnderlyingError());
    }
}

Watcher::~Watcher()
{
    if (m_epollFileDescriptor >= 0)
    {
        int result = ::close(m_epollFileDescriptor);

        if (result != 0)
        {
            LOGR::Warning{"close() ERROR", LOGR::getUnderlyingError()};
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
        throw PollingException("epoll_ctl() ERROR" + LOGR::getUnderlyingError());
    }
    m_watched.emplace(watchable->fileDescriptor(), watchable);
}

void Watcher::unwatch(std::shared_ptr<IWatchable> watchable)
{
    epoll_event event{};

    int result = ::epoll_ctl(m_epollFileDescriptor, EPOLL_CTL_DEL,
                             watchable->fileDescriptor(), &event);

    if (result != 0)
    {
        LOGR::Warning{"epoll_ctl() ERROR", LOGR::getUnderlyingError()};
    }

    m_watched.erase(watchable->fileDescriptor());
}

std::vector<std::shared_ptr<IWatchable>> Watcher::wait(int msTimeout)
{
    constexpr size_t maxEvents = 10;
    epoll_event events[maxEvents]{};

    int result = ::epoll_wait(m_epollFileDescriptor, events, maxEvents, msTimeout);

    if (result < 0)
    {
        throw PollingException("epoll_wait() ERROR" + LOGR::getUnderlyingError());
    }

    std::vector<std::shared_ptr<IWatchable>> ready;
    for (int i = 0; i < result; i++)
    {
        auto it = m_watched.find(events[i].data.fd);

        ready.push_back(it->second);
    }

    return ready;
}
