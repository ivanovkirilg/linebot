#!/bin/env python3

import sys

import socket_testing

HOST = ''
PORT = socket_testing.some_port()
MESSAGE = b'test_message'

SUT = sys.argv[1]

proc = socket_testing.launch_sut(SUT, PORT)

proc.stdout.readline()  # Wait for SUT to bind socket first

sock = socket_testing.create_socket()
sock.connect((HOST, PORT))

sock.send(MESSAGE)

received = sock.recv(100)
assert received == MESSAGE

received = sock.recv(100)
assert not received

proc.communicate(timeout=0.5) # Should already be done
