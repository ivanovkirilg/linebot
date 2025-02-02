#include "zpp_bits.h"

#include "COMM/Socket.hpp"

#include <cassert>

#include <iostream>


// method set(in double speed);
// method get(out double speed);

// method get_speed() -> double speed;
// or
// method get_speed() -> double;

constexpr int OK = 0;
constexpr int ERROR = 1;

class TestDriverServer
{
public:
    void handleRequest(
        COMM::Connection& client)
    {
        std::vector<std::byte> inBuffer = client.receive();
        auto in = zpp::bits::in(inBuffer, zpp::bits::endian::network{});
        auto [reply, out] = zpp::bits::data_out(zpp::bits::endian::network{});

        int methodCode = -1;
        in(methodCode).or_throw();

        switch (methodCode)
        {
            case 0:
            {
                double speed = 0.0;
                in(speed).or_throw();
                set(speed);
                out(OK).or_throw();
                break;
            }
            case 1:
            {
                double speed = 0.0;
                get(speed);
                out(OK).or_throw();
                out(speed).or_throw();
                break;
            }
            default:
            {
                out(ERROR).or_throw();
            }
        }

        if (not in.remaining_data().empty())
        {
            out.reset();
            out(ERROR).or_throw();
        }

        client.send(reply);
    }

protected:
    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

private:
};

class TestDriver : public TestDriverServer
{
public:
    virtual void set(double speed) override {
        std::cout << "setting speed " << speed << '\n';
        m_speed = speed;
    }
    virtual void get(double& speed) override {
        speed = m_speed;
        std::cout << "returning speed " << speed << '\n';
    }

private:
    double m_speed = 0.0;
};

int main(int argc, char *argv[])
{
    assert(argc == 3);
    int port = std::atoi(argv[1]);
    int nrOfRequests = std::atoi(argv[2]);

    TestDriver driver;

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
