#ifndef COMM_INCLUDE_COMM_SOCKET
#define COMM_INCLUDE_COMM_SOCKET

#include <string>


namespace COMM
{

class Connection
{
public:
    Connection(int fileDescriptor);
    ~Connection();

    void send(std::string message);
    std::string receive();

    operator bool();

private:
    int m_fileDescriptor = 0;
};

class Socket
{
public:
    Socket(int port);
    ~Socket();

    void bind();
    void listen(int backlog);
    [[nodiscard]] Connection accept();

    [[nodiscard]] Connection connect();

private:
    int m_fileDescriptor = 0;
    void* m_addrInfo = nullptr;
};

}

#endif // COMM_INCLUDE_COMM_SOCKET
