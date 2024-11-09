from enum import Enum, auto
from dataclasses import dataclass
import string
import re


class PunctuationKind(Enum):
    OPEN_PAREN = auto()
    CLOSE_PAREN = auto()
    COMMA = auto()
    SEMICOLON = auto()

@dataclass
class Token:
    spelling: str

@dataclass
class WordToken(Token):
    pass

@dataclass
class PunctuationToken(Token):
    kind: PunctuationKind

@dataclass
class IntegerToken(Token):
    value: int

@dataclass
class FloatToken(Token):
    value: float


def create_punct_token(spelling):
    match spelling:
        case '(':
            return PunctuationToken(spelling, PunctuationKind.OPEN_PAREN)
        case ')':
            return PunctuationToken(spelling, PunctuationKind.CLOSE_PAREN)
        case ',':
            return PunctuationToken(spelling, PunctuationKind.COMMA)
        case ';':
            return PunctuationToken(spelling, PunctuationKind.SEMICOLON)
        case _:
            raise ValueError(f'Unsupported punctuation {spelling}')

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
            tokens.append(WordToken(spelling))
        elif match.group('punct'):
            tokens.append(create_punct_token(spelling))
        elif match.group('float'):
            tokens.append(FloatToken(spelling, float(spelling)))
        elif match.group('int'):
            tokens.append(IntegerToken(spelling, int(spelling)))
        else:
            raise ValueError("Token kind not supported for: " + spelling)

    return tokens
