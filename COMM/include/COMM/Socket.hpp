#ifndef COMM_INCLUDE_COMM_SOCKET
#define COMM_INCLUDE_COMM_SOCKET

#include "COMM/Connection.hpp"
#include "COMM/IWatchable.hpp"


namespace COMM
{

///
/// Thin wrapper over network sockets
///
/// A process that wants to be available on some port should create an instance,
/// then call `bind`, `listen`, and `accept`.
///
/// To establish a connection on an already bound port, simply call `connect`.
///
class Socket : public IWatchable
{
public:
    Socket(int port);
    ~Socket();

    void bind();
    void listen(int backlog);

    ///
    /// Blocking call that waits for another process to `connect`.
    ///
    /// Normally called by the Watcher when it sees the Socket file descriptor
    /// is 'ready', meaning someone is already waiting.
    ///
    Connection accept();

    int port() const;

    /// This is the way to open a Connection to a particular (other) process.
    static Connection connect(int port);

private:
    int fileDescriptor() const override;

    int m_port = 0;
    int m_fileDescriptor = 0;
    void* m_addrInfo = nullptr;
};

} // COMM

#endif // COMM_INCLUDE_COMM_SOCKET
