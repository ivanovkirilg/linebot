#ifndef COMM_INCLUDE_COMM_CONNECTION
#define COMM_INCLUDE_COMM_CONNECTION

#include <vector>

#include "LOGR/Exception.hpp"

#include "COMM/IWatchable.hpp"


namespace COMM
{

/// The connection was politely closed by the peer
class ConnectionClosedException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};

/// A network system call resulted in an error
class NetworkException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};


class Connection : public IWatchable
{
public:
    ~Connection();

    Connection(Connection&& other);
    Connection& operator=(Connection&& other);

    void send(std::vector<std::byte> message);
    std::vector<std::byte> receive();

    operator bool();

private:
    friend class Socket;

    Connection(int fileDescriptor);

private:
    int fileDescriptor() override;

    int m_fileDescriptor = 0;
};

} // namespace COMM

#endif // COMM_INCLUDE_COMM_CONNECTION
