// client-side hand-coded file

#include "DriverClient.hpp"

#include <iostream>
#include <cassert>


int main(int argc, char *argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);

    DriverClient client(port);

    std::string method;
    do
    {
        std::cin >> method;

        if (method == "set")
        {
            double speed = 0;
            std::cin >> speed;
            client.set(speed);
        }
        else if (method == "get")
        {
            double speed = 0;
            client.get(speed);
            std::cout << speed << '\n';
        }
    }
    while (method != "quit");
}
