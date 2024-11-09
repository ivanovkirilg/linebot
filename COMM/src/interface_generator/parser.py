from enum import Enum, auto
from dataclasses import dataclass
import string
import re


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

    punct_regex = re.escape(string.punctuation)
    num_regex = '[0-9]+'

    regex = f'(?P<word>\w+)|(?P<punct>[{punct_regex}])|(?P<num>{num_regex})'
    separated_tokens = list(re.finditer(regex, translation_unit))
    # print(separated_tokens)

    for token in [tok.group() for tok in separated_tokens]:
        if token.isalpha():
            tokens.append(Token(TokenKind.WORD, token))
        elif token in string.punctuation:
            tokens.append(create_punct_token(token))
        else:
            raise ValueError("Token kind not supported for: " + token)

    return tokens
