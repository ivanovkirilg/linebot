from .tokens import *
from .syntax_tree import *

def parse_parameters(parameters: List[Token]):
    return []

def parse(tokens):
    match tokens:
        case [
                KeywordToken('method', KeywordKind.METHOD),
                WordToken() as name,
                PunctuationToken('(', PunctuationKind.OPEN_PAREN),
                *parameters,
                PunctuationToken(')', PunctuationKind.CLOSE_PAREN),
                PunctuationToken(';', PunctuationKind.SEMICOLON)
            ]:
            return [
                MethodDeclaration(
                    name.spelling,
                    ReturnSpec(DataType.VOID),
                    parameters=parse_parameters(parameters))
                ]
        case _:
            raise ValueError("Invalid structure")

    return []

