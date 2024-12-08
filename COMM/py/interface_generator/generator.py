import os.path
import re

from .syntax_tree import MethodDeclaration, ParamDirection
from .templates import *


INTERFACE_SUFFIX = '.interface'


class Generator:
    class IncludeGuard:
        def __init__(self, header_file: str, out_header: list[str]):
            self._header_file = header_file
            self._header = out_header
            self._define = self._generate_define()

        def __enter__(self):
            self._header.append('#ifndef ' + self._define)
            self._header.append('#define ' + self._define)
            self._header.append('')

        def __exit__(self, *_):
            self._header.append('')
            self._header.append('#endif // ' + self._define)

        def _generate_define(self):
            define = self._header_file.removesuffix('.hpp')
            # Punctuation to _
            define = re.sub(r'[./\: ()]', '_', define)
            # CamelCase to SHOUT_CASE
            define = re.sub(r'([a-z])([A-Z])', r'\1_\2', define)
            return define.upper()

    def __init__(self, interface_file: str, methods: list[MethodDeclaration]):
        if not interface_file.endswith(INTERFACE_SUFFIX):
            raise ValueError("Interface definition files must be suffixed "
                             f"with '.interface', and {interface_file} isn't.")
        self._interface_file = interface_file
        self._interface_name = self._get_interface_name()
        self._methods = methods
        self._parameters_per_method = {}

    def _get_interface_name(self):
        return os.path.basename(self._interface_file).removesuffix('.interface')

    def _get_header_file(self, kind: str):
        return re.sub('.interface$', f'{kind}.hpp', self._interface_file)

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

    def _generate_client_method_declarations(self) -> str:
        declarations = ''
        for method in self._methods:
            decl = CLIENT_METHOD_FORMAT.format(
                ret=method.return_spec.data_type.value,
                name=method.name,
                params=', '.join(self._get_parameters(method))
            )
            declarations += decl + '\n'
        return declarations

    def generate_client_hpp(self) -> str:
        content = []
        header_file = self._get_header_file('Client')

        with self.IncludeGuard(header_file, content):
            content.append(
                CLIENT_HEADER_FORMAT.format(
                    interface=self._interface_name,
                    methods=self._generate_client_method_declarations()
                )
            )

        return '\n'.join(content)

    def generate_server_hpp(self) -> str:
        content = []
        header_file = self._get_header_file('Server')

        with self.IncludeGuard(header_file, content):
            content.append(
                SERVER_HEADER_FORMAT.format(
                    interface=self._interface_name,
                    methods="    PLACEHOLDER"))

        return '\n'.join(content)
