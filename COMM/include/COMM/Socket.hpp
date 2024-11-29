#ifndef COMM_INCLUDE_COMM_SOCKET
#define COMM_INCLUDE_COMM_SOCKET

#include <memory>
#include <vector>
#include <map>

#include "LOGR/Exception.hpp"


namespace COMM
{

class ConnectionClosedException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};

class IWatchable
{
public:
    virtual ~IWatchable() = default;

private:
    friend class Watcher;

    virtual int fileDescriptor() = 0;
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

class Socket : public IWatchable
{
public:
    Socket(int port);
    ~Socket();

    void bind();
    void listen(int backlog);
    [[nodiscard]] Connection accept();

    int port() const;

    [[nodiscard]] static Connection connect(int port);

private:
    int fileDescriptor() override;

    int m_port = 0;
    int m_fileDescriptor = 0;
    void* m_addrInfo = nullptr;
};

class Watcher
{
public:
    Watcher();
    ~Watcher();

    void watch(std::shared_ptr<IWatchable> watchable);
    void unwatch(std::shared_ptr<IWatchable> watchable);
    std::vector<std::shared_ptr<IWatchable>> wait(int msTimeout);

private:
    int m_epollFileDescriptor = 0;
    std::map<int, std::shared_ptr<IWatchable>> m_watched;
};

}

#endif // COMM_INCLUDE_COMM_SOCKET
