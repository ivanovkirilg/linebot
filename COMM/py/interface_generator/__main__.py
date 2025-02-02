import argparse

from .lexer import tokenize
from .parser import parse
from .generator import Generator, FileKind


def main(interface_file, namespace, include_dir, source_dir):
    with open(interface_file, 'r') as f:
        interface_definition = f.read()

        tokens = tokenize(interface_definition)

        declarations = parse(tokens)

        generator = Generator(interface_file, namespace, declarations, include_dir, source_dir)

        with open(generator.get_source_path(FileKind.CLIENT), 'w') as f:
            client_cpp = generator.generate_client_cpp()
            f.write(client_cpp)

        with open(generator.get_source_path(FileKind.SERVER), 'w') as f:
            server_cpp = generator.generate_server_cpp()
            f.write(server_cpp)

        with open(generator.get_header_path(FileKind.CLIENT), 'w') as f:
            client_hpp = generator.generate_client_hpp()
            f.write(client_hpp)

        with open(generator.get_header_path(FileKind.SERVER), 'w') as f:
            server_hpp = generator.generate_server_hpp()
            f.write(server_hpp)


if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('interface_file')

    arg_parser.add_argument('-n', '--namespace',   type=str, required=True)
    arg_parser.add_argument('-i', '--include-dir', type=str, required=True)
    arg_parser.add_argument('-s', '--source-dir',  type=str, required=True)

    args = arg_parser.parse_args()

    main(args.interface_file, args.namespace, args.include_dir, args.source_dir)
