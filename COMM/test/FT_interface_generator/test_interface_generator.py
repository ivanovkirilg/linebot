#!/bin/env python3

import sys
from time import sleep

from socket_testing import launch_sut, some_port, assert_no_warnings_or_errors

HOST = ''
PORT = some_port()
EXPECTED_OUTPUT = '3 9 27'

SERVER, CLIENT = sys.argv[1], sys.argv[2]

server = launch_sut(SERVER, PORT, pipe_err=True, text=True)
sleep(.01)
client = launch_sut(CLIENT, PORT, pipe_err=True, text=True)

client_out, client_err = client.communicate()
print(client_err)

_, server_err = server.communicate()
print(server_err)

assert client_out == EXPECTED_OUTPUT, "ACTUAL %s != %s EXPECTED" % (client_out, EXPECTED_OUTPUT)

assert_no_warnings_or_errors(client_err, logr_stubbed=True)
assert_no_warnings_or_errors(server_err, logr_stubbed=True)
