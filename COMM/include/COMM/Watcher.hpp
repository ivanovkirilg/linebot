#ifndef COMM_INCLUDE_COMM_WATCHER
#define COMM_INCLUDE_COMM_WATCHER

#include "COMM/IWatchable.hpp"

#include "LOGR/Exception.hpp"

#include <memory>
#include <vector>
#include <map>


namespace COMM
{

class PollingException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};

class Watcher
{
public:
    Watcher();
    ~Watcher();

    using WatchablePtr = std::shared_ptr<IWatchable>;

    void watch(WatchablePtr watchable);
    void unwatch(WatchablePtr watchable);
    std::vector<WatchablePtr> wait(int msTimeout);

private:
    int m_epollFileDescriptor = 0;
    std::map<int, WatchablePtr> m_watched;
};

} // namespace COMM

#endif // COMM_INCLUDE_COMM_WATCHER
