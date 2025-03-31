#ifndef COMM_INCLUDE_COMM_CONNECTION
#define COMM_INCLUDE_COMM_CONNECTION

#include <vector>

#include "LOGR/Exception.hpp"

#include "COMM/IWatchable.hpp"


namespace COMM
{

///
/// The connection was unexpectedly closed by the peer
///
/// \note This is different from receiving a 'disconnect' message,
/// and applies only when a 'message' could NOT be sent/received.
///
class ConnectionClosedException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};

/// A network system call resulted in an error
class NetworkException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};

///
/// A pair of incoming & outgoing streams connected to one other process.
///
/// Created via Socket::accept or Socket::connect, allows you to send & receive
/// unstructured, undelimited chunks of bytes.
///
class [[nodiscard]] Connection : public IWatchable
{
public:
    ~Connection();

    Connection(Connection&& other);
    Connection& operator=(Connection&& other);

    ///
    /// Send a chunk of bytes to the other process.
    ///
    /// \note Does nothing to delimit this 'message' from others sent before or
    /// after it. Think of it as just a chunk in a stream of data.
    ///
    void send(std::vector<std::byte> message);

    ///
    /// Await (if necessary) and return data sent by the other process.
    ///
    /// \note The returned vector may comprise multiple chunks sent separately.
    /// \note There is currently a max chunk size of 4KiB for no good reason.
    ///
    std::vector<std::byte> receive();

    operator bool() const;

private:
    friend class Socket;

    /// Connections are only created by Socket
    Connection(int fileDescriptor);

private:
    int fileDescriptor() const override;

    int m_fileDescriptor{};
    bool m_open{};
};

} // namespace COMM

#endif // COMM_INCLUDE_COMM_CONNECTION
