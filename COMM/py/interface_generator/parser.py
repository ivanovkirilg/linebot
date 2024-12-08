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
                WordToken() as datatype,
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
                    raise ValueError("Expected ',' at " + rest_of_tokens)
        case _:
            raise ValueError("Invalid parameters structure " + parameter_tokens)

def parse(tokens: list[Token]):
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
                parameters = []
                parse_parameters(param_tokens, parameters)
                methods.append(
                    MethodDeclaration(
                        name.spelling,
                        ReturnSpec(DataType.VOID),
                        parameters
                    )
                )
            case _:
                raise ValueError(f"Invalid structure in statement '{' '.join([tok.spelling for tok in statement])}'")

    return methods

