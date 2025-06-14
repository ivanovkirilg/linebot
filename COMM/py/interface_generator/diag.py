import sys

def eprint(*args, **kwargs):
    print(*args, **kwargs, file=sys.stderr)

def highlight_error(line, start, end):
    eprint(line)
    eprint(' ' * start,
          '^' * (end - start),
          sep='')

