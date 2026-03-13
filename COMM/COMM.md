# Component COMM        {#COMM_page}

\brief Communication component.

## Usage

The primary entry-point is a CMake function:

    add_interface(XMPL Example)

which will look for `XMPL/include/XMPL/Example.interface`, and generate server
and client C++ code from it.

### Client

The client code is ready to use by including `XMPL/ExampleClient.hpp` and
linking `Example_interface`[^1]; this allows you to connect to a server
implementing this interface and make IPC calls to it.

### Server

The server code includes an abstract class in `XMPL/ExampleServer.hpp` that
must be inherited by your server implementation; which can then
[bind](#COMM::Server::bind()) to an address and enter a
[requestLoop](#COMM::Server::requestLoop()) to await and serve clients.

To exit the `requestLoop`, set `m_breakLoop` to `true`[^2].

The implementation should also link against `Example_interface`[^1].



[^1]: **TODO**: this should include the component name too, to avoid clashes.

[^2]: **TODO**: this should be a protected function instead.
