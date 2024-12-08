import argparse

from .lexer import tokenize
from .parser import parse
from .generator import Generator


def main(interface_file):
    with open(interface_file, 'r') as f:
        interface_definition = f.read()

        tokens = tokenize(interface_definition)

        declarations = parse(tokens)

        printargs = { 'sep': '\n', 'end': '\n================\n\n' }

        print('DECLARATIONS', *declarations, **printargs)

        generator = Generator(interface_file, declarations)

        client_hpp = generator.generate_client_hpp()
        print('CLIENT HEADER', client_hpp, **printargs)

        input()

        server_hpp = generator.generate_server_hpp()
        print('SERVER HEADER', server_hpp, **printargs)


if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('interface_file')
    args = arg_parser.parse_args()

    main(args.interface_file)
