#ifndef COMM_INCLUDE_COMM_SOCKET
#define COMM_INCLUDE_COMM_SOCKET

#include "COMM/Connection.hpp"
#include "COMM/IWatchable.hpp"


namespace COMM
{

class Socket : public IWatchable
{
public:
    Socket(int port);
    ~Socket();

    void bind();
    void listen(int backlog);
    Connection accept();

    int port() const;

    static Connection connect(int port);

private:
    int fileDescriptor() const override;

    int m_port = 0;
    int m_fileDescriptor = 0;
    void* m_addrInfo = nullptr;
};

} // COMM

#endif // COMM_INCLUDE_COMM_SOCKET
