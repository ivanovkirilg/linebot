// server-side GENERATED file

#include "DriverServer.hpp"
#include "zpp_bits.hpp"
#include "Socket.hpp"

#include <vector>

void DriverServer::handleRequest(
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
