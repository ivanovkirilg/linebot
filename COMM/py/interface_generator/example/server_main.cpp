// server-side hand-coded file

#include "Driver.hpp"
#include "Socket.hpp"

#include <cassert>

#include <iostream>


int main(int argc, char *argv[])
{
    assert(argc == 3);
    int port = std::atoi(argv[1]);
    int nrOfRequests = std::atoi(argv[2]);

    Driver driver;

    COMM::Socket serverSocket(port);
    serverSocket.bind();
    serverSocket.listen(1);

    std::cout << "listening" << std::endl;

    COMM::Connection client = serverSocket.accept();

    for (int i = 0; i < nrOfRequests; i++)
    {
        driver.handleRequest(client);
    }
}
