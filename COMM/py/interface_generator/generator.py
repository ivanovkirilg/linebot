from enum import Enum
import os.path
import re

from .syntax_tree import MethodDeclaration, ParamDirection, DataType


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

    def _generate_params_serialization(self,
                                       method: MethodDeclaration,
                                       direction: ParamDirection,
                                       template: str):
        return '\n'.join(
            template.format(name = param.name)
            for param in method.parameters
            if param.direction == direction
        )

    def generate_client_hpp(self) -> str:
        from .templates.client_header import FILE_TEMPLATE, ABSTRACT_METHOD_TEMPLATE, METHOD_TEMPLATE

        guard = self.IncludeGuard(self.get_output_path(FileKind.CLIENT))

        return FILE_TEMPLATE.format(
            header_guard     = guard.start,
            namespace        = self._namespace,
            interface        = self._interface_name,
            abstract_methods = self._generate_method_declarations(ABSTRACT_METHOD_TEMPLATE),
            methods          = self._generate_method_declarations(METHOD_TEMPLATE),
            end_header_guard = guard.end
        )

    def generate_server_hpp(self) -> str:
        from .templates.server_header import FILE_TEMPLATE, METHOD_TEMPLATE

        guard = self.IncludeGuard(self.get_output_path(FileKind.SERVER))

        return FILE_TEMPLATE.format(
            header_guard     = guard.start,
            namespace        = self._namespace,
            interface        = self._interface_name,
            methods          = self._generate_method_declarations(METHOD_TEMPLATE),
            end_header_guard = guard.end
        )

    def generate_client_cpp(self) -> str:
        def _generate_return_temporary(method: MethodDeclaration) -> tuple[str, str]:
            from .templates.client_source import RET_TEMPORARY_TEMPLATE, RETURN_TEMPLATE
            ret_type = method.return_spec.data_type
            if ret_type != DataType.VOID:
                return (
                    RET_TEMPORARY_TEMPLATE.format(ret=ret_type.value),
                    RETURN_TEMPLATE
                )
            else:
                return '// no return', '    // no return'

        from .templates.client_source import FILE_TEMPLATE, METHOD_TEMPLATE, \
            IN_PARAMS_TEMPLATE, OUT_PARAMS_TEMPLATE

        methods = ''
        for i, method in enumerate(self._methods):
            input_params_ser    = self._generate_params_serialization(
                method, ParamDirection.IN, IN_PARAMS_TEMPLATE)

            output_params_deser = self._generate_params_serialization(
                method, ParamDirection.OUT, OUT_PARAMS_TEMPLATE)

            ret_temporary, read_and_ret = _generate_return_temporary(method)
            method_body = METHOD_TEMPLATE.format(
                ret             = method.return_spec.data_type.value,
                namespace       = self._namespace,
                interface       = self._interface_name,
                name            = method.name,
                params          = ', '.join(self._get_parameters(method)),
                index           = (i+1),
                ret_temporary   = ret_temporary,
                write_in_params = input_params_ser,
                read_out_params = output_params_deser,
                read_and_return = read_and_ret
            )

            methods += method_body

        return FILE_TEMPLATE.format(
            namespace = self._namespace,
            interface = self._interface_name,
            methods   = methods
        )

    def generate_server_cpp(self) -> str:
        def _generate_return_temporary(method: MethodDeclaration) -> tuple[str, str, str]:
            from .templates.server_source import RET_TEMPORARY_TEMPLATE, \
                RET_ASSIGNMENT_TEMPLATE, RET_WRITE_TEMPLATE
            ret_type = method.return_spec.data_type
            if ret_type != DataType.VOID:
                return (
                    RET_TEMPORARY_TEMPLATE.format(ret=ret_type.value),
                    RET_ASSIGNMENT_TEMPLATE,
                    RET_WRITE_TEMPLATE
                )
            else:
                return '// no return', '', '// no return'

        from .templates.server_source import FILE_TEMPLATE, METHOD_TEMPLATE, \
            PARAMS_DECL_TEMPLATE, IN_PARAMS_TEMPLATE, OUT_PARAMS_TEMPLATE

        methods = ''
        for i, method in enumerate(self._methods):
            params_decl = '\n'.join(
                PARAMS_DECL_TEMPLATE.format(
                    type = param.data_type.value,
                    name = param.name
                )
                for param in method.parameters
            )

            input_params_deser = self._generate_params_serialization(
                method, ParamDirection.IN, IN_PARAMS_TEMPLATE)

            output_params_ser  = self._generate_params_serialization(
                method, ParamDirection.OUT, OUT_PARAMS_TEMPLATE)

            ret_temp, ret_assignment, ret_write = _generate_return_temporary(method)

            method_body = METHOD_TEMPLATE.format(
                index              = i + 1,
                ret_temporary      = ret_temp,
                params_declaration = params_decl,
                read_in_params     = input_params_deser,
                ret_assignment     = ret_assignment,
                name               = method.name,
                params             = ', '.join(param.name for param in method.parameters),
                write_out_params   = output_params_ser,
                ret_write          = ret_write
            )

            methods += method_body

        return FILE_TEMPLATE.format(
            interface = self._interface_name,
            namespace = self._namespace,
            methods   = methods
        )
