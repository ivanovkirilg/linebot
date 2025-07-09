#include "ExampleImpl.hpp"

#include "LOGR/ILogger.hpp"
#include "LOGR/Trace.hpp"

#include <cassert>
#include <cstdlib>


int main(int argc, char* argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);

    auto log = LOGR::ILogger::create("XMPL_server", LOGR::StubSelection::STUB);
    LOGR::Trace trace{};

    XMPL::ExampleImpl server;

    server.bind(port);
    trace.log("Server bound to", port);

    server.requestLoop();
}
