#include "COMM/Socket.hpp"

#include <cassert>
#include <cstdlib>

#include <iostream>

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);
    COMM::Connection server = COMM::Socket::connect(port);

    std::string message = "request";
    server.send(message);

    message = server.receive();

    std::cout << message;
}
