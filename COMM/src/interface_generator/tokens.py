from enum import Enum, auto
from dataclasses import dataclass

class PunctuationKind(Enum):
    OPEN_PAREN = auto()
    CLOSE_PAREN = auto()
    COMMA = auto()
    SEMICOLON = auto()
    RIGHT_ARROW = auto()

PUNCTUATION = {
    '(':  PunctuationKind.OPEN_PAREN,
    ')':  PunctuationKind.CLOSE_PAREN,
    ',':  PunctuationKind.COMMA,
    ';':  PunctuationKind.SEMICOLON,
    '->': PunctuationKind.RIGHT_ARROW,
}

class KeywordKind(Enum):
    METHOD = auto()
    IN = auto()
    OUT = auto()

KEYWORDS = {
    'method': KeywordKind.METHOD,
    'in': KeywordKind.IN,
    'out': KeywordKind.OUT,
}

@dataclass
class Token:
    spelling: str

@dataclass
class WordToken(Token):
    pass

@dataclass
class KeywordToken(WordToken):
    kind: KeywordKind = None

    def __post_init__(self):
        if self.kind is None:
            self.kind = KEYWORDS[self.spelling]

@dataclass
class PunctuationToken(Token):
    kind: PunctuationKind = None

    def __post_init__(self):
        if self.kind is None:
            try:
                self.kind = PUNCTUATION[self.spelling]
            except KeyError:
                raise ValueError(f'Unsupported punctuation {self.spelling}')

@dataclass
class IntegerToken(Token):
    value: int = None

    def __post_init__(self):
        if self.value is None:
            self.value = int(self.spelling)

@dataclass
class FloatToken(Token):
    value: float = None

    def __post_init__(self):
        if self.value is None:
            self.value = float(self.spelling)
