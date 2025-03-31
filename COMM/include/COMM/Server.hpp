#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_SERVER
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_SERVER

#include <memory>
#include <string>

#include "COMM/Socket.hpp"

namespace COMM
{

/// Base class for executing external requests from multiple Connections
class Server
{
public:
    /// Allows you to create an object, then bind it later
    Server() = default;
    /// Not yet implemented
    Server(const std::string& localAddress);
    /// Immediately bind to the specified port
    Server(int port);

    virtual ~Server() = default;

    /// Not yet implemented
    void bind(const std::string& localAddress);
    /// Allow other processes to Socket::connect to the specified port
    void bind(int port);

    ///
    /// Listen for requests from other processes
    ///
    /// Watches for new connections, and for method requests to be answered
    /// via 'handleRequest', until m_breakLoop is true.
    void requestLoop();

protected:
    enum class ConnectionStatus
    {
        CONNECTED,
        DISCONNECTED
    };

    virtual ConnectionStatus handleRequest(COMM::Connection& client) = 0;

    bool m_breakLoop{};

private:
    std::string m_localAddress;
    std::shared_ptr<COMM::Socket> m_connectionSocket;
};

} // COMM

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_SERVER
