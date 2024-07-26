#include "COMM/Socket.hpp"

#include <iostream>

int main()
{
    COMM::Socket serverSocket(9001);

    COMM::Connection server = serverSocket.connect();

    while (true)
    {
        std::string message;
        std::cin >> message;

        server.send(message);

        std::cout << server.receive() << std::endl;

        if (message == "close")
        {
            break;
        }
    }
}
