from enum import Enum
import os.path
import re

from .syntax_tree import MethodDeclaration, ParamDirection
from .templates import *


INTERFACE_SUFFIX = '.interface'


class FileKind(Enum):
    CLIENT = "Client"
    SERVER = "Server"


class Generator:
    class IncludeGuard:
        @property
        def start(self):
            return f'#ifndef {self._define}\n#define {self._define}'

        @property
        def end(self):
            return f'#endif // {self._define}'

        def __init__(self, header_file: str):
            self._define = self._generate_define(header_file)

        def _generate_define(self, header_file: str):
            define = header_file.removesuffix('.hpp')
            # Punctuation to _
            define = re.sub(r'[./\: ()]', '_', define)
            # CamelCase to SHOUT_CASE
            define = re.sub(r'([a-z])([A-Z])', r'\1_\2', define)
            return define.upper()

    def __init__(self,
                 interface_file: str,
                 namespace: str,
                 methods: list[MethodDeclaration],
                 output_dir: str):
        if not interface_file.endswith(INTERFACE_SUFFIX):
            raise ValueError("Interface definition files must be suffixed "
                             f"with '.interface', and {interface_file} isn't.")
        self._interface_file = interface_file
        self._interface_name = self._get_interface_name()
        self._namespace = namespace
        self._methods = methods
        self._parameters_per_method = {}

        self._output_dir = output_dir

    def _get_interface_name(self):
        return os.path.basename(self._interface_file).removesuffix('.interface')

    def get_output_path(self, kind: FileKind, extension='hpp'):
        return os.path.join(
            self._output_dir,
            f'{self._get_interface_name()}{kind.value}.{extension}')

    def _get_parameters(self, method: MethodDeclaration):
        if method.name in self._parameters_per_method.keys():
            return self._parameters_per_method[method.name]

        params = []
        for param in method.parameters:
            ref = '&' if param.direction == ParamDirection.OUT else ''
            param_string = '{}{} {}'.format(
                param.data_type.value, ref, param.name)
            params.append(param_string)
        self._parameters_per_method[method.name] = params

        return params

    def _generate_method_declarations(self, format: str) -> str:
        declarations = ''
        for method in self._methods:
            decl = format.format(
                ret=method.return_spec.data_type.value,
                name=method.name,
                params=', '.join(self._get_parameters(method))
            )
            declarations += decl + '\n'
        return declarations

    def generate_client_hpp(self) -> str:
        guard = self.IncludeGuard(self.get_output_path(FileKind.CLIENT))

        return CLIENT_HEADER_FORMAT.format(
                    header_guard=guard.start,
                    namespace=self._namespace,
                    interface=self._interface_name,
                    methods=self._generate_method_declarations(CLIENT_METHOD_FORMAT),
                    abstract_methods=self._generate_method_declarations(CLIENT_ABSTRACT_METHOD_FORMAT),
                    end_header_guard=guard.end)

    def generate_server_hpp(self) -> str:
        guard = self.IncludeGuard(self.get_output_path(FileKind.SERVER))

        return SERVER_HEADER_FORMAT.format(
            header_guard=guard.start,
            namespace=self._namespace,
            interface=self._interface_name,
            methods=self._generate_method_declarations(SERVER_METHOD_FORMAT),
            end_header_guard=guard.end
        )

    def generate_client_cpp(self) -> str:
        methods = ''
        for i, method in enumerate(self._methods):
            input_params_ser = '\n'.join(
                CLIENT_SERIALIZE_INPUT_FORMAT.format(name=param.name)
                for param in method.parameters
                if param.direction == ParamDirection.IN
            )

            output_params_deser = '\n'.join(
                CLIENT_DESERIALIZE_OUTPUT_FORMAT.format(name=param.name)
                for param in method.parameters
                if param.direction == ParamDirection.OUT
            )

            method_body = CLIENT_SOURCE_METHOD_FORMAT.format(
                ret=method.return_spec.data_type.value,
                namespace=self._namespace,
                interface=self._interface_name,
                name=method.name,
                params=', '.join(self._get_parameters(method)),
                index=(i+1),
                input_params_serialization=input_params_ser,
                output_params_deserialization=output_params_deser,
            )

            methods += method_body

        return CLIENT_SOURCE_FORMAT.format(
            namespace=self._namespace,
            interface=self._interface_name,
            methods=methods
        )

    def generate_server_cpp(self) -> str:
        methods = ''
        for i, method in enumerate(self._methods):
            params_decl = '\n'.join(
                SERVER_PARAMS_DECLARATION_FORMAT.format(type=param.data_type.value, name=param.name)
                for param in method.parameters
            )

            input_params_deser = '\n'.join(
                SERVER_DESERIALIZE_INPUT_FORMAT.format(name=param.name)
                for param in method.parameters
                if param.direction == ParamDirection.IN
            )

            output_params_ser = '\n'.join(
                SERVER_SERIALIZE_OUTPUT_FORMAT.format(name=param.name)
                for param in method.parameters
                if param.direction == ParamDirection.OUT
            )

            method_body = SERVER_SOURCE_METHOD_FORMAT.format(
                index=(i+1),
                params_declaration=params_decl,
                input_params_deserialization=input_params_deser,
                name=method.name,
                params=', '.join(param.name for param in method.parameters),
                output_params_serialization=output_params_ser,
            )

            methods += method_body

        return SERVER_SOURCE_FORMAT.format(
            interface=self._interface_name,
            namespace=self._namespace,
            methods=methods
        )
