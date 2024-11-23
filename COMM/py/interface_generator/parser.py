from typing import Iterable, List
from .tokens import *
from .syntax_tree import *


def parse_parameters(parameter_tokens: Iterable[Token], out_params: List[Parameter]):
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

def parse(tokens):
    match tokens:
        case [
                KeywordToken('method', KeywordKind.METHOD),
                WordToken() as name,
                PunctuationToken('(', PunctuationKind.OPEN_PAREN),
                *param_tokens,
                PunctuationToken(')', PunctuationKind.CLOSE_PAREN),
                PunctuationToken(';', PunctuationKind.SEMICOLON)
            ]:
            parameters = []
            parse_parameters(param_tokens, parameters)
            return [
                MethodDeclaration(
                    name.spelling,
                    ReturnSpec(DataType.VOID),
                    parameters
                )
            ]
        case _:
            raise ValueError("Invalid structure")

    return []

