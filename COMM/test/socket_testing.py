import csv
import os
import socket
import subprocess
from enum import Enum

class LogLevel(Enum):
    TRACE     = 'TRACE'
    WARNING   = 'WARN '
    EXCEPTION = 'EXC  '

def some_port():
    sock = create_socket()
    sock.bind( ('', 0) )
    port = sock.getsockname()[1]
    sock.close()
    return port

def create_socket():
    return socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def launch_sut(sut_exe, *args, debug=False, pipe_err=False, **kwargs):
    cmd = [sut_exe] + list(map(str, args))

    if debug:
        debug_port = some_port()
        cmd = ['gdbserver', f':{debug_port}'] + cmd
        print(f">>>>>>>>>>>>>>> Debug SUT with:\n"
              f"gdb {sut_exe} -ex 'target remote :{debug_port}'\n"
              f"<<<<<<<<<<<<<<< Or abort with: kill {os.getpid()}")

    return subprocess.Popen(cmd,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE if pipe_err else None,
                            **kwargs)

def get_trace_of_levels(trace: str, levels: set[LogLevel], logr_stubbed: bool) -> list[str]:
    loglevel_field = 1 + (1 * logr_stubbed)

    return [';'.join(line)
            for line in csv.reader(trace.splitlines(), delimiter=';')
            if LogLevel(line[loglevel_field]) in levels]

def assert_no_warnings_or_errors(trace, logr_stubbed: bool):
    warnings_or_errors = get_trace_of_levels(
        trace, {LogLevel.WARNING, LogLevel.EXCEPTION}, logr_stubbed)
    assert not warnings_or_errors, "Unexpected warnings/errors:\n  %s" % '\n  '.join(warnings_or_errors)

