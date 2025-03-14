#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_SERVER
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_SERVER

#include <memory>
#include <string>

#include "COMM/Socket.hpp"

namespace COMM
{

enum class ConnectionStatus
{
    CONNECTED,
    DISCONNECTED
};

class Server
{
public:
    Server() = default;
    Server(const std::string& localAddress);
    Server(int port);

    virtual ~Server() = default;

    void bind(const std::string& localAddress);
    void bind(int port);

    void requestLoop();
    virtual ConnectionStatus handleRequest(COMM::Connection& client) = 0;

protected:
    bool m_breakLoop{};

private:
    std::string m_localAddress;
    std::shared_ptr<COMM::Socket> m_connectionSocket;
};

} // COMM

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_SERVER
