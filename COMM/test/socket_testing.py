import subprocess
import socket

def some_port():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind( ('', 0) )
    port = sock.getsockname()[1]
    sock.close()
    return port

def create_socket():
    return socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def launch_sut(sut_exe, *args):
    cmd = [sut_exe] + list(map(str, args))
    return subprocess.Popen(cmd, stdout=subprocess.PIPE)
