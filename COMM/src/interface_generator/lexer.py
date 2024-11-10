import string
import re

from tokens import *

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
        case '->':
            return PunctuationToken(spelling, PunctuationKind.RIGHT_ARROW)
        case _:
            raise ValueError(f'Unsupported punctuation {spelling}')

def tokenize(translation_unit: str) -> list[Token]:
    tokens = []

    # method, in, out
    keyword_regex = r"\bmethod\b|\bin\b|\bout\b"

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

    # Any punctuation symbol,
    # but only some are supported;
    # '->' as a special case
    punct_regex = f'->|[{ re.escape(string.punctuation) }]'

    regex = '|'.join(
        [
            f'(?P<keyword>{keyword_regex})',
            f'(?P<word>{word_regex})',
            f'(?P<float>{float_regex})',
            f'(?P<int>{int_regex})',
            f'(?P<punct>{punct_regex})'
        ]
    )

    for match in re.finditer(regex, translation_unit):
        spelling = match.group()

        if match.group('keyword'):
            tokens.append(KeywordToken(spelling, KEYWORDS[spelling]))

        elif match.group('word'):
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
