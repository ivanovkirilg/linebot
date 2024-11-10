from tokens import *
from syntax_tree import *

def parse(tokens):
    match tokens:
        case [
            KeywordToken('method', KeywordKind.METHOD),
            name,
            PunctuationToken('(', PunctuationKind.OPEN_PAREN),
            PunctuationToken(')', PunctuationKind.CLOSE_PAREN),
            PunctuationToken(';', PunctuationKind.SEMICOLON)
            ]:
            return [
                MethodDeclaration(
                    name.spelling,
                    ReturnSpec(DataType.VOID),
                    parameters=[])
                ]
        case _:
            raise ValueError("Invalid structure")

    return []

