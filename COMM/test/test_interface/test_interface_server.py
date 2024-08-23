#!/bin/env python3

import subprocess
import socket
import sys
import struct

def some_port():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind( ('', 0) )
    port = sock.getsockname()[1]
    sock.close()
    return port


HOST = ''
PORT = some_port()
NR_OF_REQUESTS = 2

SUT = sys.argv[1]

proc = subprocess.Popen([SUT, str(PORT), str(NR_OF_REQUESTS)],
                        stdout=subprocess.PIPE)

proc.stdout.readline()  # Wait for SUT to bind socket first

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

# Method code 0 (set), in speed 0.2
req = struct.pack('!id', 0, 0.2)
sock.send(req)

reply = sock.recv(1024)
reply = struct.unpack('!i', reply)
assert reply == (0,) # OK

# Method code 1 (get)
req = struct.pack('!i', 1)
sock.send(req)

reply = sock.recv(1024)
reply = struct.unpack('!id', reply)
assert reply == (0, 0.2) # OK, out speed

received = sock.recv(100)
assert not received

proc.communicate(timeout=0.5) # Should already be done
