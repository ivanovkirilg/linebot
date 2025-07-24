#include "COMM/Server.hpp"

#include "LOGR/ILogger.hpp"

#include <cassert>
#include <cstdlib>

#include "zpp_bits.h"


class TestServer : public COMM::Server
{
    using Server::Server;

    using RemainActive = bool;

    RemainActive implementation(double& number)
    {
        if (number < 0)
        {
            m_breakLoop = true;
            return false;
        }

        number *= 1.5;
        return true;
    }

    ConnectionStatus handleRequest(COMM::Connection &client) override
    {
        auto request = client.receive();
        auto in = zpp::bits::in(request, zpp::bits::endian::network{});

        double num{};
        in(num).or_throw();

        if (implementation(num) == RemainActive{true})
        {
            auto [reply, out] = zpp::bits::data_out(zpp::bits::endian::network{});
            out(num).or_throw();

            client.send(reply);

            return ConnectionStatus::CONNECTED;
        }
        else
        {
            return ConnectionStatus::DISCONNECTED;
        }
    }
};

int main(int argc, char* argv[])
{
    using namespace COMM;

    assert(argc == 2);
    int port = std::atoi(argv[1]);

    auto logger = LOGR::ILogger::create("COMM_FT", LOGR::StubSelection::STUB);

    TestServer server{port};

    server.requestLoop();
}
