import argparse
from dataclasses import dataclass, field
import re


@dataclass
class Argument:
    direction: str
    typename: str
    name: str

@dataclass
class Method:
    name: str
    args: list[Argument] = field(default_factory=list)


def parse(interface_definition: str):
    declarations = interface_definition.split(';')
    methods = []
    for decl in declarations[:-1]:
        tokens = re.split('\s+|[()]', decl)
        tokens = filter(None, tokens)

        tok = next(tokens, None)
        if not tok:
            raise Exception("Empty declaration: " + decl)

        assert tok == 'method', "Only methods are supported!"
        
        tok = next(tokens, None)
        assert tok, "No method name!"
        assert tok.isalnum(), "Method names must be alphanumeric!"
        method = Method(tok)

        tok = next(tokens, None)
        while tok:
            direction = tok
            assert direction in ['in', 'out', 'inout'], "Invalid argument direction!"

            tok = next(tokens, None)
            assert tok, "Incomplete argument!"
            typename = tok

            tok = next(tokens, None)
            assert tok, "Incomplete argument!"
            argname = tok

            method.args.append(Argument(direction, typename, argname))

            tok = next(tokens, None)

        print(method)
        methods.append(method)

    print(methods)
    return methods


def generate(methods: list[Method]):
    for method in methods:
        pass


def main(interface_file):
    with open(interface_file, 'r') as f:
        interface_definition = f.read()
        methods = parse(interface_definition)
        
        generate(methods)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('interface_file')
    args = parser.parse_args()

    main(args.interface_file)
