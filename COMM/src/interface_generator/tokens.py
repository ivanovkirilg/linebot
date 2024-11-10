from enum import Enum, auto
from dataclasses import dataclass

class PunctuationKind(Enum):
    OPEN_PAREN = auto()
    CLOSE_PAREN = auto()
    COMMA = auto()
    SEMICOLON = auto()
    RIGHT_ARROW = auto()

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
    kind: KeywordKind

@dataclass
class PunctuationToken(Token):
    kind: PunctuationKind

@dataclass
class IntegerToken(Token):
    value: int

@dataclass
class FloatToken(Token):
    value: float
