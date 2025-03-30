import unittest
import difflib
import os

from ..syntax_tree import MethodDeclaration, ParamDirection, ReturnSpec, DataType, Parameter
from ..generator import Generator


class TestGenerator(unittest.TestCase):
    def _get_expected_file(self, file_name):
        return os.path.join(os.path.dirname(__file__), 'example', file_name)

    def setUp(self):
        self.methods = [
            MethodDeclaration(
                name='set',
                return_spec=ReturnSpec(data_type=DataType.VOID, name=None),
                parameters=[
                    Parameter(direction=ParamDirection.IN, data_type=DataType.DOUBLE, name='value')
                ]
            ),
            MethodDeclaration(
                name='get',
                return_spec=ReturnSpec(data_type=DataType.DOUBLE, name=None),
                parameters=[]
            ),
            MethodDeclaration(
                name='calculate',
                return_spec=ReturnSpec(data_type=DataType.VOID, name=None),
                parameters=[
                    Parameter(direction=ParamDirection.OUT, data_type=DataType.DOUBLE, name='square'),
                    Parameter(direction=ParamDirection.OUT, data_type=DataType.DOUBLE, name='cube')
                ]
            )
        ]

        self.example_interface = self._get_expected_file('Example.interface')

        self.generator = Generator(self.example_interface, 'XMPL', self.methods, '/tmp')

    def test_generates_client_hpp(self):
        self._assert_generates(
            'ExampleClient.hpp',
            self.generator.generate_client_hpp
        )

    def test_generates_server_hpp(self):
        self._assert_generates(
            'ExampleServer.hpp',
            self.generator.generate_server_hpp
        )

    def test_generates_client_cpp(self):
        self._assert_generates(
            'ExampleClient.cpp',
            self.generator.generate_client_cpp
        )

    def test_generates_server_cpp(self):
        self._assert_generates(
            'ExampleServer.cpp',
            self.generator.generate_server_cpp
        )

    def _assert_generates(self, expected_file_name, generator_method_under_test):
        actual: str = generator_method_under_test()

        with open(self._get_expected_file(expected_file_name), 'r') as file:
            expected = file.read()

        self.assertEqual(
            expected, actual,
            '\n'.join(
                difflib.unified_diff(
                    expected.splitlines(),
                    actual.splitlines()
                )
            )
        )
