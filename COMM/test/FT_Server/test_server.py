#!/bin/env python3

import socket
import struct
import sys

import socket_testing

HOST = 'localhost'
PORT = socket_testing.some_port()

SUT = sys.argv[1]

proc = socket_testing.launch_sut(SUT, PORT)

clientA = socket.create_connection((HOST, PORT))
clientB = socket.create_connection((HOST, PORT))

clientA.send(struct.pack('!d', 1))
assert clientA.recv(16) == struct.pack('!d', 1.5)

clientB.send(struct.pack('!d', 2))
assert clientB.recv(16) == struct.pack('!d', 3)

clientA.send(struct.pack('!d', -1))
assert clientA.recv(16) == b''

clientB.send(struct.pack('!d', -1))
assert clientB.recv(16) == b''

# sent = clientB.send(struct.pack('!d', -1))
# assert sent == 0, f'sent {sent}'

out, err = proc.communicate(timeout=0.5) # Should already be done
