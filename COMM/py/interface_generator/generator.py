import os.path
import re

from .syntax_tree import MethodDeclaration
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

    def _get_interface_name(self):
        return os.path.basename(self._interface_file).removesuffix('.interface')

    def _get_header_file(self):
        return re.sub('.interface$', 'Server.hpp', self._interface_file)

    def generate_client_hpp(self) -> str:
        content = []
        header_file = re.sub('.interface$', 'Client.hpp', self._interface_file)

        with self.IncludeGuard(header_file, content):
            content.append(
                CLIENT_HEADER_FORMAT.format(
                    interface=self._interface_name,
                    methods="    PLACEHOLDER"))

        return '\n'.join(content)

    def generate_server_hpp(self) -> str:
        content = []
        header_file = self._get_header_file()

        with self.IncludeGuard(header_file, content):
            content.append(
                SERVER_HEADER_FORMAT.format(
                    interface=self._interface_name,
                    methods="    PLACEHOLDER"))

        return '\n'.join(content)
