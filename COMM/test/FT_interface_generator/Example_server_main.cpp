#include "ExampleImpl.hpp"

#include "LOGR/ILogger.hpp"
#include "LOGR/Trace.hpp"

#include <cassert>
#include <cstdlib>


int main(int argc, char* argv[])
{
    assert(argc == 2);
    int port = std::atoi(argv[1]);

    auto log = LOGR::ILoggerStub::create("XMPL_server");
    LOGR::Trace trace{};

    XMPL::ExampleImpl server;

    server.bind(port);
    trace.log("Server bound to", port);

    try
    {
        server.requestLoop();
    }
    catch (LOGR::Exception& exc)
    {
        exc.handle("Here");
    }
}
