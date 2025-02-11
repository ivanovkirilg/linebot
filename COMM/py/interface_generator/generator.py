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
        def __init__(self, header_file: str, out_header: list[str]):
            self._header_file = header_file
            self._header = out_header
            self._define = self._generate_define()

        def __enter__(self):
            self._header.append('#ifndef ' + self._define)
            self._header.append('#define ' + self._define)

        def __exit__(self, *_):
            self._header.append('#endif // ' + self._define)

        def _generate_define(self):
            define = self._header_file.removesuffix('.hpp')
            # Punctuation to _
            define = re.sub(r'[./\: ()]', '_', define)
            # CamelCase to SHOUT_CASE
            define = re.sub(r'([a-z])([A-Z])', r'\1_\2', define)
            return define.upper()

    def __init__(self,
                 interface_file: str,
                 namespace: str,
                 methods: list[MethodDeclaration],
                 include_dir: str,
                 source_dir: str):
        if not interface_file.endswith(INTERFACE_SUFFIX):
            raise ValueError("Interface definition files must be suffixed "
                             f"with '.interface', and {interface_file} isn't.")
        self._interface_file = interface_file
        self._interface_name = self._get_interface_name()
        self._namespace = namespace
        self._methods = methods
        self._parameters_per_method = {}

        self._include_dir = include_dir
        self._source_dir = source_dir

    def _get_interface_name(self):
        return os.path.basename(self._interface_file).removesuffix('.interface')

    def get_header_path(self, kind: FileKind):
        return os.path.join(self._include_dir, f'{self._get_interface_name()}{kind.value}.hpp')

    def get_source_path(self, kind: FileKind):
        return os.path.join(self._source_dir, f'{self._get_interface_name()}{kind.value}.cpp')

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
        content = []
        header_file = self.get_header_path(FileKind.CLIENT)

        with self.IncludeGuard(header_file, content):
            content.append(
                CLIENT_HEADER_FORMAT.format(
                    namespace=self._namespace,
                    interface=self._interface_name,
                    methods=self._generate_method_declarations(CLIENT_METHOD_FORMAT),
                    abstract_methods=self._generate_method_declarations(CLIENT_ABSTRACT_METHOD_FORMAT)
                )
            )

        return '\n'.join(content)

    def generate_server_hpp(self) -> str:
        content = []
        header_file = self.get_header_path(FileKind.SERVER)

        with self.IncludeGuard(header_file, content):
            content.append(
                SERVER_HEADER_FORMAT.format(
                    namespace=self._namespace,
                    interface=self._interface_name,
                    methods=self._generate_method_declarations(SERVER_METHOD_FORMAT),
                )
            )

        return '\n'.join(content)

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
                index=i,
                input_params_serialization=input_params_ser,
                output_params_deserialization=output_params_deser,
            )

            methods += method_body

        return CLIENT_SOURCE_FORMAT.format(
            header=self.get_header_path(FileKind.CLIENT),
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
                index=i,
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
