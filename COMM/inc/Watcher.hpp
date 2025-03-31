#ifndef COMM_INCLUDE_COMM_WATCHER
#define COMM_INCLUDE_COMM_WATCHER

#include "COMM/IWatchable.hpp"

#include "LOGR/Exception.hpp"

#include <memory>
#include <vector>
#include <map>


namespace COMM
{

/// A polling system call resulted in an error
class PollingException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};

///
/// Indicates which Watchable(s) are 'ready'
///
/// By watching multiple Connections, you may respond to whichever has sent
/// data without blocking on a 'receive' from the others.
/// Likewise, by also watching one or more Sockets you may 'accept' only as
/// needed (when another process 'connect's), and in the meantime await data
/// to 'receive' from your Connections.
///
class Watcher
{
public:
    Watcher();
    ~Watcher();

    using WatchablePtr = std::shared_ptr<IWatchable>;

    ///
    /// Subscribe to the watchable's ready state
    ///
    /// \note This shares ownership of the watchable with the Watcher,
    /// so it cannot be destroyed until it is unwatched (or the Watcher
    /// is also destroyed)
    ///
    void watch(WatchablePtr watchable);

    /// Unsubscribe from this watchable, also allowing it to be destroyed
    void unwatch(WatchablePtr watchable);

    /// Block until one or more watchables are 'ready' (but at most timeout_ms)
    [[nodiscard]] std::vector<WatchablePtr> wait(int timeout_ms);

private:
    int m_epollFileDescriptor = -1;
    std::map<int, WatchablePtr> m_watched;
};

} // namespace COMM

#endif // COMM_INCLUDE_COMM_WATCHER
