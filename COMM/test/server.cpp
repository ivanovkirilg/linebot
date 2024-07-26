#include "COMM/Socket.hpp"

#include <cassert>
#include <cstdlib>

#include <iostream>

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);
    COMM::Socket socket(port);

    socket.bind();
    socket.listen(1);

    std::cout << "listening" << std::endl;

    COMM::Connection client = socket.accept();

    std::string message;
    message = client.receive();

    client.send(message);
}
