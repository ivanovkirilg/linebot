#include "COMM/Server.hpp"
#include "COMM/Socket.hpp"
#include "COMM/Watcher.hpp"

#include <iostream>

namespace
{

constexpr int CONNECTIONS_BACKLOG = 16;

}

using namespace COMM;

Server::Server(const std::string& localAddress)
{
    bind(localAddress);
}

Server::Server(int port)
{
    bind(port);
}

void Server::bind(const std::string& localAddress)
{
    m_localAddress = localAddress;

    m_connectionSocket = std::make_shared<Socket>(0);

    std::cout << "Server port: " << m_connectionSocket->port() << '\n';

    m_connectionSocket->bind();
    m_connectionSocket->listen(CONNECTIONS_BACKLOG);
}

void Server::bind(int port)
{
    m_localAddress = std::to_string(port);

    m_connectionSocket = std::make_shared<Socket>(port);

    std::cout << "Server port: " << m_connectionSocket->port() << '\n';

    m_connectionSocket->bind();
    m_connectionSocket->listen(CONNECTIONS_BACKLOG);
}

void Server::unbind()
{
    // TODO:
    // m_connectionSocket->close();
}

void Server::requestLoop()
{
    Watcher watcher;

    watcher.watch(m_connectionSocket);

    while (not m_breakLoop)
    {
        std::cout << "Watching for connections...\n";
        auto ready = watcher.wait(1500);

        for (std::shared_ptr<IWatchable> rdy : ready)
        {
            Socket* acc = dynamic_cast<Socket*>(rdy.get());
            Connection* conn = dynamic_cast<Connection*>(rdy.get());
            if (acc)
            {
                std::cout << "Accepting...\n";
                auto connection = std::make_shared<Connection>(acc->accept());
                watcher.watch(connection);
                // m_connections.emplace(connection->fileDescriptor(), connection);
            }
            else if (conn)
            {
                std::cout << "Handling...\n";
                try
                {
                    handleRequest(*conn);
                }
                catch (ConnectionClosedException& exc)
                {
                    watcher.unwatch(rdy);
                    exc.handle("Unwatching client");
                }
            }
            else
            {
                throw std::runtime_error("Invalid watchable");
                rdy.get();
            }
        }
    }
}
