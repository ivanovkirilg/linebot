#ifndef COMM_INCLUDE_COMM_SOCKET
#define COMM_INCLUDE_COMM_SOCKET

#include <string>


namespace COMM
{

class Connection
{
public:
    ~Connection();

    void send(std::string message);
    std::string receive();

    operator bool();

private:
    Connection(int fileDescriptor);
    friend class Socket;

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

    [[nodiscard]] static Connection connect(int port);

private:
    int m_fileDescriptor = 0;
    void* m_addrInfo = nullptr;
};

}

#endif // COMM_INCLUDE_COMM_SOCKET
