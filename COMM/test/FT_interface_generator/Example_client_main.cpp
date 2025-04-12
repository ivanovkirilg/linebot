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

    auto log = LOGR::ILogger::create("XMPL_client", LOGR::StubSelection::STUB);
    LOGR::Trace trace;

    XMPL::ExampleClient client{port};
    trace.log("Connected on port", port);

    client.set(3);
    trace.log("Set value");

    double square{};
    double cube{};
    client.calculate(square, cube);
    trace.log("Got ^2 =", square, "^3 =", cube);

    std::cout << client.get() << ' ' << square << ' ' << cube;
}
