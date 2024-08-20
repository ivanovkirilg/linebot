#include "zpp_bits.hpp"

#include <iostream>


// method set(in double speed);
// method get(out double speed);

class TestDriverServer
{
public:
    std::vector<std::byte> request(
        const std::vector<std::byte>& inBuffer)
    {
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
                break;
            }
            case 1:
            {
                double speed = 0.0;
                get(speed);
                out(speed).or_throw();
                break;
            }
        }

        return reply;
    }

protected:
    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

private:
};

class TestDriverClient
{
public:
    TestDriverClient(TestDriverServer* server) : m_serverConnection(server) {};

    void set(double speed)
    {
        auto [inargs, out] = zpp::bits::data_out(zpp::bits::endian::network{});
        out(0).or_throw();
        out(speed).or_throw();
        auto outargs = m_serverConnection->request(inargs);
    }

    void get(double& speed)
    {
        auto [inargs, out] = zpp::bits::data_out(zpp::bits::endian::network{});
        out(1).or_throw();
        auto outargs = m_serverConnection->request(inargs);
        auto in = zpp::bits::in(outargs, zpp::bits::endian::network{});
        in(speed).or_throw();
    }

private:
    TestDriverServer* m_serverConnection;
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

int main()
{
    TestDriver driver;

    TestDriverServer* driverInterface = &driver;

    // auto [args, out] = zpp::bits::data_out(zpp::bits::endian::network{});

    // auto reply = driverInterface->request(1, args);

    // auto in = zpp::bits::in(reply, zpp::bits::endian::network{});
    // double speed;
    // in(speed).or_throw();
    // std::cout << speed << '\n';

    // out(0.2).or_throw();
    // reply = driverInterface->request(0, args);
    // std::cout << "reply size: " << reply.size() << '\n';

    // auto reply1 = driverInterface->request(1, args);

    // auto in1 = zpp::bits::in(reply1, zpp::bits::endian::network{});
    // in1(speed).or_throw();
    // std::cout << speed << '\n';

    TestDriverClient client(driverInterface);

    double speed = 0;
    client.get(speed);
    std::cout << "speed: " << speed << '\n';
    client.set(0.8);

    client.get(speed);
    std::cout << "speed: " << speed << '\n';
}
