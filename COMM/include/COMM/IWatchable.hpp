#ifndef COMM_INCLUDE_COMM_IWATCHABLE
#define COMM_INCLUDE_COMM_IWATCHABLE

namespace COMM
{

class IWatchable
{
public:
    virtual ~IWatchable() = default;

private:
    friend class Watcher;

    virtual int fileDescriptor() = 0;
};

} // namespace COMM

#endif // COMM_INCLUDE_COMM_IWATCHABLE
