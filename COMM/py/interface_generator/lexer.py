import string
import re

from .tokens import *

def tokenize(translation_unit: str) -> list[Token]:
    tokens = []

    # Letters, digits, underscore;
    # beginning with a letter
    word_regex = r'[a-zA-Z][a-zA-Z0-9_]*'

    # Digits preceded by a point,
    # optionally preceded by a whole part,
    # optionally preceded by a sign,
    # optionally an exponent
    float_regex = r'[-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?'

    # Digits, optionally preceded by a sign
    int_regex = r'[-+]?[0-9]+'

    # Any punctuation symbol,
    # but only some are supported;
    # '->' as a special case
    punct_regex = f'->|[{ re.escape(string.punctuation) }]'

    regex = '|'.join(
        [
            f'(?P<word>{word_regex})',
            f'(?P<float>{float_regex})',
            f'(?P<int>{int_regex})',
            f'(?P<punct>{punct_regex})'
        ]
    )

    for line_nr, line in enumerate(translation_unit.splitlines()):
        for matched in re.finditer(regex, line):
            spelling = matched.group()

            if matched.group('word'):
                if spelling in DATA_TYPE:   tok_type = DataTypeToken
                elif spelling in KEYWORDS:  tok_type = KeywordToken
                else:                       tok_type = WordToken
            elif matched.group('punct'):    tok_type = PunctuationToken
            elif matched.group('float'):    tok_type = FloatToken
            elif matched.group('int'):      tok_type = IntegerToken
            else:
                raise ValueError("Token kind not supported for: " + spelling)

            loc = Location(line_nr + 1, matched.span())
            tokens.append( tok_type(spelling).at(loc) )

    return tokens
