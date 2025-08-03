import sys
import os.path
import re

import plotly.express as px

from argparse import ArgumentParser
from collections import defaultdict
from dataclasses import dataclass, field
from enum import Enum

class LogLevel(Enum):
    TRACE = '0'
    WARNING = '1'
    EXCEPTION = '2'

@dataclass
class LogLine:
    level: LogLevel
    timestamp: float
    thread_id: str
    src_file: str
    line: int
    func: str
    message: str

@dataclass
class Trace:
    start: LogLine
    end: LogLine | None = None
    interim: list[LogLine] = field(default_factory=list)

@dataclass
class Warning:
    log: LogLine

@dataclass
class Exception_:
    id: int
    raised: LogLine
    handled: LogLine | None = None

@dataclass
class ThreadLogs:
    traces: list[Trace] = field(default_factory=list)
    warnings: list[Warning] = field(default_factory=list)
    exceptions: list[Exception_] = field(default_factory=list)

type StructuredLogs = defaultdict[str, dict[str, ThreadLogs]]

def parse_args():
    parser = ArgumentParser(description="Visualize LOGR CSV files")
    parser.add_argument("files", nargs="+", help="Log files to view")
    return parser.parse_args()

def parse_task_name(file: str) -> str:
    match = re.match(
        r'LOGR_(\w+)_\d+.csv',
        os.path.basename(file)
        )
    if match is None:
        raise ValueError(f'File name "{file}" does not look like a LOGR file.')
    else:
        return match.group(1)

def read_logs(files: list[str]) -> dict[str, list[str]]:
    logs = {}
    for file in files:
        task = parse_task_name(file)
        with open(file, 'r') as f:
            logs[task] = f.readlines()
    return logs

def parse_log(log_lines: list[str]) -> list[LogLine]:
    structured = []
    for log in log_lines:
        fields = log.split(';') # FIXME messages containing ;
        structured.append(
            LogLine(
                level     = LogLevel( fields[0] ),
                timestamp = float(    fields[1] ),
                thread_id =           fields[2],
                src_file  =           fields[3],
                line      = int(      fields[4] ),
                func      =           fields[5],
                message   =           fields[6],
            )
        )
    return structured

def separate_threads(logs: list[LogLine]) -> defaultdict[str, list[LogLine]]:
    threads = defaultdict(list)
    for log in logs:
        threads[log.thread_id].append(log)
    return threads

def structure_logs(logs: list[LogLine]) -> ThreadLogs:
    structured = ThreadLogs()
    open_traces: list[Trace] = []
    open_exceptions: dict[int, Exception_] = {}
    for log in logs:
        if log.level == LogLevel.TRACE:
            if log.message.startswith('v'):
                open_traces.append(Trace(log))

            elif log.message.startswith('|'):
                last = open_traces[-1]
                assert log.func == last.start.func and \
                       log.src_file == last.start.src_file
                last.interim.append(log)

            elif log.message.startswith('^'):
                last = open_traces[-1]
                assert log.func == last.start.func and \
                       log.src_file == last.start.src_file
                last.end = log
                structured.traces.append(open_traces.pop())

            else:
                raise ValueError(f'Invalid message "{log.message}" in trace log {log}')

        elif log.level == LogLevel.WARNING:
            structured.warnings.append(Warning(log))

        elif log.level == LogLevel.EXCEPTION:
            raised = re.match(r'> \[(\d+)\]', log.message)
            handled = re.match(r'< \[(\d+)\]', log.message)
            if raised:
                exc_id = int(raised.group(1))
                assert exc_id not in open_exceptions
                open_exceptions[exc_id] = Exception_(exc_id, log)

            elif handled:
                exc_id = int(handled.group(1))
                exc = open_exceptions.pop(exc_id)
                exc.handled = log
                structured.exceptions.append(exc)

    if open_traces or open_exceptions:
        print("Warning: unterminated logs found - "
              "either program crashed, or parsing failed")
        structured.traces.extend(open_traces)
        structured.exceptions.extend(open_exceptions.values())

    return structured

def visualize(structured_logs: StructuredLogs):
    from datetime import datetime

    traces = []

    for task, threads in structured_logs.items():
        for thread_id, logs in threads.items():
            for trace in logs.traces:
                if trace.end:
                    traces.append(
                        {
                            'Task': task,
                            'Thread': thread_id,
                            'Func': trace.start.func,
                            'Start': datetime.fromtimestamp(trace.start.timestamp),
                            'End': datetime.fromtimestamp(trace.end.timestamp),
                            'Duration': trace.end.timestamp - trace.start.timestamp,
                        }
                    )

    fig = px.timeline(traces,
                      x_start='Start',
                      x_end='End',
                      y='Func',
                      facet_row='Thread',
                      hover_data=('Func', 'Task', 'Duration'),
    )

    fig = fig.update_yaxes(autorange='reversed', matches=None, showticklabels=False)
    fig.show()

def main(files):
    try:
        logs = read_logs(files)
    except ValueError as e:
        print(f"{e}", file=sys.stderr)
        exit(1)
    except FileNotFoundError as e:
        print(f"Log file not found: {e}", file=sys.stderr)
        exit(1)

    structured_logs: StructuredLogs = defaultdict(dict)
    for task, log in logs.items():
        parsed = parse_log(log)
        threads = separate_threads(parsed)
        for thread_id, thread_logs in threads.items():
            structured_logs[task][thread_id] = structure_logs(thread_logs)

    visualize(structured_logs)

if __name__ == "__main__":
    args = parse_args()
    main(args.files)
