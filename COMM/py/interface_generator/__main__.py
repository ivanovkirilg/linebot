import argparse

from .lexer import tokenize
from .parser import parse

def main(interface_file):
    with open(interface_file, 'r') as f:
        interface_definition = f.read()

    tokens = tokenize(interface_definition)

    definitions = parse(tokens)

    print(definitions)


if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('interface_file')
    args = arg_parser.parse_args()

    main(args.interface_file)
