import argparse

from .lexer import tokenize
from .parser import Parser, ParseError
from .generator import Generator, FileKind

def parse_arguments():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('interface_file')

    arg_parser.add_argument('-V', '--verbose',     action='store_true')
    arg_parser.add_argument('-n', '--namespace',   type=str, required=True)
    arg_parser.add_argument('-o', '--output-dir',  type=str, required=True)

    return arg_parser.parse_args()

class Application:
    def __init__(self, args):
        self._verbose        = args.verbose
        self._interface_file = args.interface_file
        self._namespace      = args.namespace
        self._output_dir     = args.output_dir

        with open(self._interface_file, 'r') as f:
            self._interface_definition = f.read()

    def parse(self, tokens):
        parser = Parser(self._interface_definition.splitlines())

        try:
            declarations = parser.parse(tokens)
        except ParseError as err:
            print(f'Failed to parse {self._interface_file}')
            if self._verbose:
                print(f'{err}')
            exit(1)

        return declarations

    def main(self):
        tokens = tokenize(self._interface_definition, self._interface_file)

        declarations = self.parse(tokens)

        if self._verbose:
            print(declarations)

        generator = Generator(self._interface_file, self._namespace, declarations, self._output_dir)

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
    args = parse_arguments()

    app = Application(args)

    app.main()
