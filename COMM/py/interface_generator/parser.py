from typing import Iterable
from .tokens import *
from .syntax_tree import *

def diagnose_parameter(tokens: list[Token]):
    match tokens:
        case [
            KeywordToken('in') | KeywordToken('out') as direction,
            WordToken() as other
            ]:
            if other.spelling in DATA_TYPE:
                print(f"Missing parameter name for '{direction.spelling} {other.spelling}'")
            else:
                print(f"Missing data type for parameter '{direction.spelling} {other.spelling}'")
        case _:
            print(">>>> DIAGNOSTICS FAILED <<<<")

def separate(tokens: list[Token], delimiter: Token) -> list[list[Token]]:
    sections = []
    start = end = 0
    while start is not None:
        try:
            end = tokens.index(delimiter, start)
            sections.append(tokens[start:end])
            start = end + 1
        except ValueError:
            if start != len(tokens):
                sections.append(tokens[start:])
            start = None
    return sections

def parse_parameters(parameter_tokens: list[Token], out_params: list[Parameter]):
    parameters = separate(parameter_tokens, PunctuationToken(','))
    for parameter in parameters:
        match parameter:
            case []:
                return
            case [
                    KeywordToken() as direction,
                    WordToken() as datatype,
                    WordToken() as name,
                ]:
                out_params.append(
                    Parameter(
                        PARAM_DIRECTION[direction.kind],
                        DATA_TYPE[datatype.spelling],
                        name.spelling)
                )
            case _:
                diagnose_parameter(parameter)
                raise ValueError(f"Invalid parameters structure {parameter}")

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
    statements = separate(tokens, PunctuationToken(';'))

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
                WordToken() as return_type
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

