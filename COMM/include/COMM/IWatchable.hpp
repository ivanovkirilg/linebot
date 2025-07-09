#ifndef COMM_INCLUDE_COMM_IWATCHABLE
#define COMM_INCLUDE_COMM_IWATCHABLE

namespace COMM
{

/// A network-related entity that may be 'watched' for a 'ready' state.
class IWatchable
{
public:
    virtual ~IWatchable() = default;

private:
    friend class Watcher;

    virtual int fileDescriptor() const = 0;
};

} // namespace COMM

#endif // COMM_INCLUDE_COMM_IWATCHABLE
