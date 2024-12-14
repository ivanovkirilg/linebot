// client-side hand-coded file

#include "DriverClient.hpp"

#include <iostream>
#include <cassert>


int main(int argc, char *argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);

    DRVR::DriverClient client(port);

    std::string method;
    do
    {
        std::cin >> method;

        if (method == "set")
        {
            double speed = 0;
            std::cin >> speed;
            try
            {
                client.set(speed);
            }
            catch (std::exception& exc)
            {
                std::cout << "Caught " << exc.what() << std::endl;
            }
        }
        else if (method == "get")
        {
            double speed = 0;
            try
            {
                client.get(speed);
            }
            catch (std::exception& exc)
            {
                std::cout << "Caught " << exc.what() << std::endl;
            }
            std::cout << speed << '\n';
        }
    }
    while (method != "quit");
}
