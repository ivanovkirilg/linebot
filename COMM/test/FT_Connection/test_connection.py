#!/bin/env python3

import sys

import socket_testing

HOST = ''
PORT = socket_testing.some_port()
EXPECTED_MESSAGE = b'request'
REPLY = b'reply'

SUT = sys.argv[1]

sock = socket_testing.create_socket()
sock.bind((HOST, PORT))
sock.listen(2)

proc = socket_testing.launch_sut(SUT, PORT)

conn, _ = sock.accept()

received = conn.recv(100)
assert received == EXPECTED_MESSAGE, "ACTUAL %s != %s EXPECTED" % (received, EXPECTED_MESSAGE)

conn.send(REPLY)

out, _ = proc.communicate(timeout=3)
assert out == REPLY, "ACTUAL %s != %s EXPECTED" % (out, REPLY)

sock.close()
conn.close()
