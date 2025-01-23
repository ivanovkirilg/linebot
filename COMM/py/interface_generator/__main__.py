import argparse

from .lexer import tokenize
from .parser import parse
from .generator import Generator


def main(interface_file, namespace, include_dir, source_dir):
    with open(interface_file, 'r') as f:
        interface_definition = f.read()

        tokens = tokenize(interface_definition)

        declarations = parse(tokens)

        generator = Generator(interface_file, namespace, declarations)

        client_cpp = generator.generate_client_cpp()

        server_cpp = generator.generate_server_cpp()

        client_hpp = generator.generate_client_hpp()

        server_hpp = generator.generate_server_hpp()


if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('interface_file')

    # TODO nargs makes it a list :/
    arg_parser.add_argument('-n', '--namespace', nargs=1, required=True)
    arg_parser.add_argument('-i', '--include-dir', nargs=1, required=True)
    arg_parser.add_argument('-s', '--source-dir', nargs=1, required=True)
    args = arg_parser.parse_args()

    main(args.interface_file, *args.namespace, *args.include_dir, *args.source_dir)
