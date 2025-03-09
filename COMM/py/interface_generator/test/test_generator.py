import unittest

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
        server_hpp = generator.generate_server_hpp()
        client_cpp = generator.generate_client_cpp()
        server_cpp = generator.generate_server_cpp()

        # TODO
        self.assertIn('namespace XMPL', client_hpp)
        self.assertIn('class ExampleClient', client_hpp)

        self.assertIn('namespace XMPL', server_hpp)
        self.assertIn('class ExampleServer', server_hpp)

        self.assertIn('void XMPL::ExampleClient::set(double speed)', client_cpp)
        self.assertIn('void XMPL::ExampleServer::handleRequest(Connection& client)', server_cpp)
