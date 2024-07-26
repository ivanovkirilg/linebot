#include "COMM/Socket.hpp"

#include <iostream>

int main()
{
    COMM::Socket socket(9001);

    socket.bind();
    socket.listen(1);

    COMM::Connection client = socket.accept();

    std::string message;
    while (true)
    {
        try
        {
            message = client.receive();
        }
        catch (std::runtime_error& exc)
        {
            std::cout << "connection closed\n";
            exit(0);
        }

        if (message == "request")
        {
            client.send("reply");
        }
        else if (message == "close")
        {
            client.send("ack");
            client.~Connection();
            break;
        }
        else
        {
            client.send("error");
        }
    }
}
