#include "COMM/Socket.hpp"
#include "Watcher.hpp"

#include "LOGR/ILogger.hpp"

#include <cassert>
#include <cstdlib>

#include <iostream>


int main(int argc, char* argv[])
{
    using namespace COMM;

    assert(argc == 3);
    int portA = std::atoi(argv[1]);
    int portB = std::atoi(argv[2]);

    auto logger = LOGR::ILogger::create("COMM_FT", LOGR::StubSelection::STUB);

    std::vector<std::byte> ok;
    ok.push_back(static_cast<std::byte>('o'));
    ok.push_back(static_cast<std::byte>('k'));

    auto connectionA = std::make_shared<Connection>(Socket::connect(portA));
    auto connectionB = std::make_shared<Connection>(Socket::connect(portB));

    Watcher watcher;
    watcher.watch(connectionA);
    watcher.watch(connectionB);

    std::string incomingMessage;

    while (incomingMessage != "end")
    {
        auto ready = watcher.wait(15);

        for (Watcher::WatchablePtr rdy : ready)
        {
            auto readyConnection = std::dynamic_pointer_cast<Connection>(rdy);

            incomingMessage.clear();
            auto incomingBuffer = readyConnection->receive();
            for (std::byte b : incomingBuffer)
            {
                incomingMessage += static_cast<char>(b);
            }

            std::cout << incomingMessage << '\n';

            readyConnection->send(ok);
        }
    }
}
