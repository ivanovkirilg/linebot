#include "COMM/Socket.hpp"

#include <iostream>

int main()
{
    COMM::Connection server = COMM::Socket::connect(9001);

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
