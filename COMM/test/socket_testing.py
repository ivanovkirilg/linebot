import subprocess
import socket
import os

def some_port():
    sock = create_socket()
    sock.bind( ('', 0) )
    port = sock.getsockname()[1]
    sock.close()
    return port

def create_socket():
    return socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def launch_sut(sut_exe, *args, debug=False):
    cmd = [sut_exe] + list(map(str, args))

    if debug:
        debug_port = some_port()
        cmd = ['gdbserver', f':{debug_port}'] + cmd
        print(f">>>>>>>>>>>>>>> Debug SUT with:\n"
              f"gdb {sut_exe} -ex 'target remote :{debug_port}'\n"
              f"<<<<<<<<<<<<<<< Or abort with: kill {os.getpid()}")

    return subprocess.Popen(cmd, stdout=subprocess.PIPE)
