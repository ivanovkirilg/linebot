import argparse

from .lexer import tokenize
from .parser import parse
from .generator import Generator, FileKind


def main(verbose, interface_file, namespace, output_dir):
    with open(interface_file, 'r') as f:
        interface_definition = f.read()

        tokens = tokenize(interface_definition)

        declarations = parse(tokens)
        if verbose:
            print(declarations)

        generator = Generator(interface_file, namespace, declarations, output_dir)

        with open(generator.get_output_path(FileKind.CLIENT, 'cpp'), 'w') as f:
            client_cpp = generator.generate_client_cpp()
            f.write(client_cpp)

        with open(generator.get_output_path(FileKind.SERVER, 'cpp'), 'w') as f:
            server_cpp = generator.generate_server_cpp()
            f.write(server_cpp)

        with open(generator.get_output_path(FileKind.CLIENT), 'w') as f:
            client_hpp = generator.generate_client_hpp()
            f.write(client_hpp)

        with open(generator.get_output_path(FileKind.SERVER), 'w') as f:
            server_hpp = generator.generate_server_hpp()
            f.write(server_hpp)


if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('interface_file')

    arg_parser.add_argument('-V', '--verbose',     action='store_true')
    arg_parser.add_argument('-n', '--namespace',   type=str, required=True)
    arg_parser.add_argument('-o', '--output-dir',  type=str, required=True)

    args = arg_parser.parse_args()

    main(args.verbose, args.interface_file, args.namespace, args.output_dir)
