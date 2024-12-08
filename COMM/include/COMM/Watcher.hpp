#ifndef COMM_INCLUDE_COMM_WATCHER
#define COMM_INCLUDE_COMM_WATCHER

#include <memory>
#include <vector>
#include <map>

#include "COMM/IWatchable.hpp"

namespace COMM
{

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
