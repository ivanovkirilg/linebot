#include "ExampleClient.hpp"

#include "LOGR/ILogger.hpp"
#include "LOGR/Trace.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>


int main(int argc, char* argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);

    auto log = LOGR::ILoggerStub::create("XMPL_client");
    LOGR::Trace trace;

    XMPL::ExampleClient client{port};
    trace.log("Connected on port", port);

    client.set(15);
    trace.log("Set speed");

    double speed{};
    client.get(speed);
    trace.log("Got speed", speed);

    std::cout << speed;
}
