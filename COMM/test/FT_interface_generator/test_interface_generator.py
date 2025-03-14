#!/bin/env python3

import sys
from time import sleep

import socket_testing

HOST = ''
PORT = socket_testing.some_port()
EXPECTED_OUTPUT = b'15'

SERVER, CLIENT = sys.argv[1], sys.argv[2]

server = socket_testing.launch_sut(SERVER, PORT)
sleep(.01)
client = socket_testing.launch_sut(CLIENT, PORT)

out, err = client.communicate()

assert out == EXPECTED_OUTPUT, "ACTUAL %s != %s EXPECTED" % (out, EXPECTED_OUTPUT)
assert not err, "Unexpected stderr output: %s" % err
