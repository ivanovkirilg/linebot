#include "COMM/Server.hpp"

#include "COMM/Socket.hpp"
#include "Watcher.hpp"

#include "LOGR/Exception.hpp"
#include "LOGR/Trace.hpp"

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

void Server::requestLoop()
{
    LOGR::Trace trace(m_localAddress, m_connectionSocket->port());

    Watcher watcher;

    watcher.watch(m_connectionSocket);

    while (not m_breakLoop)
    {
        auto ready = watcher.wait(1500);

        for (std::shared_ptr<IWatchable> rdy : ready)
        {
            if (auto acc = dynamic_cast<Socket*>(rdy.get()))
            {
                trace.log("Accepting connection on port", acc->port());

                auto connection = std::make_shared<Connection>(acc->accept());
                watcher.watch(connection);
            }
            else if (auto conn = dynamic_cast<Connection*>(rdy.get()))
            {
                try
                {
                    if (handleRequest(*conn) == ConnectionStatus::DISCONNECTED)
                    {
                        watcher.unwatch(rdy);
                    }
                }
                catch (ConnectionClosedException& exc)
                {
                    watcher.unwatch(rdy);
                    exc.handle("Unwatched client");
                }
            }
            else
            {
                throw LOGR::Exception("Invalid watchable");
            }
        }
    }
}
