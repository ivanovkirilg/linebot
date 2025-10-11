import sys

def eprint(*args, **kwargs):
    print(*args, **kwargs, file=sys.stderr)

def error(message: str, line_nr: int):
    eprint(f'[Error] {message} on line {line_nr}')

def highlight_error(line, start, end):
    eprint(line)
    eprint(' ' * start,
          '^' * (end - start),
          sep='')

def failed():
    eprint(">>>> DIAGNOSTICS FAILED <<<<")
