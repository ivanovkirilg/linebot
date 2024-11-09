from enum import Enum, auto
from dataclasses import dataclass
import string
import re


class TokenKind(Enum):
    WORD = auto()
    PUNCTUATION = auto()
    FLOAT = auto()
    INT = auto()

class PunctuationKind(Enum):
    OPEN_PAREN = auto()
    CLOSE_PAREN = auto()
    COMMA = auto()
    SEMICOLON = auto()

class NumberKind(Enum):
    INTEGER = auto()
    FLOATING_POINT = auto()

@dataclass
class Punctuation:
    kind: PunctuationKind

@dataclass
class Number:
    kind: NumberKind
    value: int | float

@dataclass
class Token:
    kind: TokenKind
    spelling: str
    value: None | int | float = None
    punctKind: None | PunctuationKind = None


def create_punct_token(spelling):
    tok = Token(TokenKind.PUNCTUATION, spelling)
    match spelling:
        case '(':
            tok.punctKind = PunctuationKind.OPEN_PAREN
        case ')':
            tok.punctKind = PunctuationKind.CLOSE_PAREN
        case ',':
            tok.punctKind = PunctuationKind.COMMA
        case ';':
            tok.punctKind = PunctuationKind.SEMICOLON
        case _:
            raise ValueError(f'Unsupported punctuation {spelling}')
    return tok

def tokenize(translation_unit: str) -> list[Token]:
    tokens = []

    # Letters, digits, underscore;
    # beginning with a letter
    word_regex = '[a-zA-Z][a-zA-Z0-9_]*'

    # Digits preceded by a point,
    # optionally preceded by a whole part,
    # optionally preceded by a sign,
    # optionally an exponent
    float_regex = '[-+]?\d*\.\d+([eE][-+]?\d+)?'

    # Digits, optionally preceded by a sign
    int_regex = '[-+]?\d+'

    # Any punctuation; but only some are supported
    punct_regex = re.escape(string.punctuation)

    regex = '|'.join( [ f'(?P<word>{word_regex})',
                        f'(?P<float>{float_regex})',
                        f'(?P<int>{int_regex})',
                        f'(?P<punct>[{punct_regex}])'
                    ] )

    for match in re.finditer(regex, translation_unit):
        spelling = match.group()

        if match.group('word'):
            tokens.append(Token(TokenKind.WORD, spelling))
        elif match.group('punct'):
            tokens.append(create_punct_token(spelling))
        elif match.group('float'):
            tokens.append(Token(TokenKind.FLOAT, spelling, float(spelling)))
        elif match.group('int'):
            tokens.append(Token(TokenKind.INT, spelling, int(spelling)))
        else:
            raise ValueError("Token kind not supported for: " + spelling)

    return tokens
