from typing import Iterable
from .tokens import *
from .syntax_tree import *


def separate_statements(tokens: list[Token]) -> list[list[Token]]:
    statements = []
    start = end = 0
    while start is not None:
        try:
            end = tokens.index(PunctuationToken(';'), start)
            statements.append(tokens[start:end])
            start = end + 1
        except ValueError:
            start = None
    return statements

def parse_parameters(parameter_tokens: Iterable[Token], out_params: list[Parameter]):
    match parameter_tokens:
        case []:
            return
        case [
                KeywordToken() as direction,
                DataTypeToken() as datatype,
                WordToken() as name,
                *rest_of_tokens
            ]:
            out_params.append(
                Parameter(
                    PARAM_DIRECTION[direction.kind],
                    DATA_TYPE[datatype.spelling],
                    name.spelling)
            )
            if rest_of_tokens:
                if rest_of_tokens[0] == PunctuationToken(','):
                    parse_parameters(rest_of_tokens[1:], out_params)
                else:
                    raise ValueError(f"Expected ',' at {rest_of_tokens}")
        case _:
            raise ValueError(f"Invalid parameters structure {parameter_tokens}")

def construct_method(name: WordToken,
                     param_tokens: list[Token],
                     return_spec: ReturnSpec) -> MethodDeclaration:
    parameters = []
    parse_parameters(param_tokens, parameters)

    return MethodDeclaration(
        name.spelling,
        return_spec,
        parameters
    )


def parse(tokens: list[Token]) -> list[MethodDeclaration]:
    statements = separate_statements(tokens)

    methods = []

    for statement in statements:
        match statement:
            case [
                    KeywordToken('method'),
                    WordToken() as name,
                    PunctuationToken('('),
                    *param_tokens,
                    PunctuationToken(')')
                ]:
                methods.append(
                    construct_method(name, param_tokens, ReturnSpec(DataType.VOID))
                )

            case [
                KeywordToken('method'),
                WordToken() as name,
                PunctuationToken('('),
                *param_tokens,
                PunctuationToken(')'),

                PunctuationToken('->'),
                DataTypeToken() as return_type
            ]:
                # TODO diagnostics on KeyError
                return_spec = ReturnSpec(DATA_TYPE[return_type.spelling])

                methods.append(
                    construct_method(name, param_tokens, return_spec)
                )

            case _:
                raise ValueError("Invalid structure in statement: "
                                 + ' '.join([tok.spelling for tok in statement]))

    return methods

