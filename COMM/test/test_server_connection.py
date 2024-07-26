#!/bin/env python3

import subprocess
import socket
import sys

def some_port():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind( ('', 0) )
    port = sock.getsockname()[1]
    sock.close()
    return port


HOST = ''
PORT = some_port()
MESSAGE = b'test_message'

SUT = sys.argv[1]

proc = subprocess.Popen([SUT, str(PORT)], stdout=subprocess.PIPE)

proc.stdout.readline()  # Wait for SUT to bind socket first

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

sock.send(MESSAGE)

received = sock.recv(100)
assert received == MESSAGE

received = sock.recv(100)
assert not received

proc.communicate(timeout=0.5) # Should already be done
