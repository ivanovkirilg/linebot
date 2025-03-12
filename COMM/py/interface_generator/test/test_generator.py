import unittest
import difflib

from ..syntax_tree import MethodDeclaration, ParamDirection, ReturnSpec, DataType, Parameter
from ..generator import Generator


class TestGenerator(unittest.TestCase):
    methods = [
        MethodDeclaration(
            name='set',
            return_spec=ReturnSpec(data_type=DataType.VOID, name=None),
            parameters=[
                Parameter(direction=ParamDirection.IN, data_type=DataType.DOUBLE, name='speed')
            ]
        ),
        MethodDeclaration(
            name='get',
            return_spec=ReturnSpec(data_type=DataType.VOID, name=None),
            parameters=[
                Parameter(direction=ParamDirection.OUT, data_type=DataType.DOUBLE, name='speed')
            ]
        )
    ]

    def test_generates(self):
        generator = Generator('example/Example.interface', 'XMPL', self.methods, '/tmp')

        client_hpp = generator.generate_client_hpp()
        client_cpp = generator.generate_client_cpp()
        server_hpp = generator.generate_server_hpp()
        server_cpp = generator.generate_server_cpp()

        expected_files = [
            'interface_generator/test/example/' + f for f in (
                'ExampleClient.hpp',
                'ExampleClient.cpp',
                'ExampleServer.hpp',
                'ExampleServer.cpp'
            )
        ]

        def read_file(f):
            with open(f, 'r') as file:
                return file.read()

        expected_contents = [read_file(f) for f in expected_files]

        for actual, expected, title in zip([client_hpp, client_cpp, server_hpp, server_cpp],
                                           expected_contents,
                                           ['Client Header', 'Client Source', 'Server Header', 'Server Source']):
            self.assertEqual(expected, actual,
                             f"\n{title} differs:\n" +
                             '\n'.join(difflib.unified_diff(expected.splitlines(), actual.splitlines())))
