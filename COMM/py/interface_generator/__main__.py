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
        tokens = tokenize(self._interface_definition)

        declarations = self.parse(tokens)

        if self._verbose:
            print(declarations)

        generator = Generator(self._interface_file, self._namespace, declarations, self._output_dir)

        output_matrix = {
            FileKind.CLIENT: {
                'hpp': generator.generate_client_hpp,
                'cpp': generator.generate_client_cpp,
            },
            FileKind.SERVER: {
                'hpp': generator.generate_server_hpp,
                'cpp': generator.generate_server_cpp,
            },
        }

        for file_kind, extensions in output_matrix.items():
            for extension, generate in extensions.items():
                with open(generator.get_output_path(file_kind, extension), 'w') as f:
                    content = generate()
                    f.write(content)

if __name__ == '__main__':
    args = parse_arguments()

    app = Application(args)

    app.main()
