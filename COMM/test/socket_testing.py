import subprocess
import socket
import csv
from typing import Union
from enum import Enum

class LogLevel(Enum):
    TRACE = '0'
    WARNING = '1'
    EXCEPTION = '2'

def some_port():
    sock = create_socket()
    sock.bind( ('', 0) )
    port = sock.getsockname()[1]
    sock.close()
    return port

def create_socket():
    return socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def launch_sut(sut_exe, *args, pipe_err=False, **kwargs):
    cmd = [sut_exe] + list(map(str, args))
    return subprocess.Popen(cmd,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE if pipe_err else None,
                            **kwargs)

def get_trace_of_levels(trace: str, levels: Union[LogLevel], logr_stubbed: bool) -> bool:
    loglevel_field = 0 + (1 * logr_stubbed)

    return [';'.join(line)
            for line in csv.reader(trace.splitlines(), delimiter=';')
            if LogLevel(line[loglevel_field]) in levels]

def assert_no_warnings_or_errors(trace, logr_stubbed: bool, expected: list = []):
    warnings_or_errors = get_trace_of_levels(
        trace, {LogLevel.WARNING, LogLevel.EXCEPTION}, logr_stubbed)
    unexpected = [line for line in warnings_or_errors
                  if not any(exp in line for exp in expected)]
    assert not unexpected, "Unexpected warnings/errors:\n  %s" % '\n  '.join(unexpected)

