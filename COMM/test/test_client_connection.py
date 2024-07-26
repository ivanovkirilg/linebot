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
EXPECTED_MESSAGE = b'request'
REPLY = b'reply'

SUT = sys.argv[1]

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT))
sock.listen(2)

proc = subprocess.Popen([SUT, str(PORT)], stdout=subprocess.PIPE)

conn, _ = sock.accept()

received = conn.recv(100)
assert received == EXPECTED_MESSAGE, "ACTUAL %s != %s EXPECTED" % (received, EXPECTED_MESSAGE)

conn.send(REPLY)

out, _ = proc.communicate(timeout=3)
assert out == REPLY, "ACTUAL %s != %s EXPECTED" % (out, REPLY)

sock.close()
conn.close()
