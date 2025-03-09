The initial concepts for COMM interface generation were based on having a
separate 'watcher' thread for handling connections, as evidenced by the present
`server_thread` diagrams. This was initially driven by the idea of leaving the
main thread free to do 'something' other than serving requests, potentially
calling the 'Server Object' in the meantime (see `async_request.pu`).
Struggling to think of a use case for this, I simplified to executing the
actual methods on the main thread (in a 'requestLoop' for clients), and kept
the 'watcher' thread responsible for receiving request messages and sending
replies (see `sync_request_loop.pu`).

At that point, I still failed to see any tangible benefit from having a second
thread if it just had to wait for the main one. So I oversimplified to the
`single_thread` version. But what the `sync_request_loop.pu` diagram gets wrong
(and `request_loop.pu` misses) is which calls are concurrent.

Sockets are streams of data and one `recv` may yield any number of messages
sent by the respective connection. Therefore these should be received (and
potentially parsed) separately from the `requestLoop`.

A containment until a more robust protocol and message parsing are implemented
is to ensure that a single client connection will always await the server's
reply before sending another request. This is done by adding a mutex member
and locking it for the entirety of each method.
