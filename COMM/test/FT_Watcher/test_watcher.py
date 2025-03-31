#!/bin/env python3

import sys

import socket_testing

HOST = ''
PORT_A = socket_testing.some_port()
PORT_B = socket_testing.some_port()

SUT = sys.argv[1]

sockA = socket_testing.create_socket()
sockA.bind((HOST, PORT_A))
sockA.listen(2)

sockB = socket_testing.create_socket()
sockB.bind((HOST, PORT_B))
sockB.listen(2)

proc = socket_testing.launch_sut(SUT, PORT_A, PORT_B)

connA, _ = sockA.accept()
connB, _ = sockB.accept()

connA.send(b'first message')
assert connA.recv(16) == b'ok'

connB.send(b'second message')
assert connB.recv(16) == b'ok'

connB.send(b'third message')
assert connB.recv(16) == b'ok'

connA.send(b'end')
assert connA.recv(16) == b'ok'

out, err = proc.communicate(timeout=0.5) # Should already be done

EXPECTED = b'first message\nsecond message\nthird message\nend\n'

assert out == EXPECTED, "ACTUAL %s != %s EXPECTED" % (out, EXPECTED)
