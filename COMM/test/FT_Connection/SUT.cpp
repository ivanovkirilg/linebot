#include "COMM/Socket.hpp"

#include <cassert>
#include <cstdlib>

#include <iostream>


int main(int argc, char* argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);
    COMM::Connection server = COMM::Socket::connect(port);

    std::string outgoingMessage = "request";
    std::vector<std::byte> outgoingBuffer;
    for (char b : outgoingMessage)
    {
        outgoingBuffer.push_back(static_cast<std::byte>(b));
    }

    server.send(outgoingBuffer);

    std::vector<std::byte> incomingBuffer = server.receive();
    std::string incomingMessage;
    for (std::byte b : incomingBuffer)
    {
        incomingMessage += static_cast<char>(b);
    }

    std::cout << incomingMessage;
}
