from enum import Enum, auto
from dataclasses import dataclass
import string


class TokenKind(Enum):
    WORD = auto()
    PUNCTUATION = auto()
    LITERAL = auto()
    OPERATOR = auto()

class PunctuationKind(Enum):
    OPEN_PAREN = auto()
    CLOSE_PAREN = auto()
    COMMA = auto()
    SEMICOLON = auto()


@dataclass
class Token:
    kind: TokenKind
    spelling: str
    value: None | int | float = None
    punctKind: None | PunctuationKind = None


def tokenize(translation_unit: str) -> list[Token]:
    tokens = []
    
    for token in translation_unit.split():
        if token.isalpha():
            tokens.append(Token(TokenKind.WORD, token))
        elif token in string.punctuation:
            tokens.append(Token(TokenKind.PUNCTUATION, token))
        else:
            raise Exception("Token kind not supported")

    return tokens
