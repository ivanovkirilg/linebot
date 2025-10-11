from enum import Enum, auto
from dataclasses import dataclass, field

class DataType(Enum):
    VOID = 'void'

    BYTE = 'byte'
    BOOL = 'bool'

    INT = 'int'
    FLOAT = 'float'
    DOUBLE = 'double'

    CHAR = 'char'
    STRING = 'string'

DATA_TYPE = {
    'void': DataType.VOID,
    'byte': DataType.BYTE,
    'bool': DataType.BOOL,
    'int': DataType.INT,
    'float': DataType.FLOAT,
    'double': DataType.DOUBLE,
    'char': DataType.CHAR,
    'string': DataType.STRING,
}

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
class Location:
    line_nr: int
    column_range: tuple[int, int]

@dataclass
class Token:
    spelling: str
    location: Location = field(
        default_factory=lambda: Location(0, (0, 0)),
        init=False,   # allow Tokens without location, esp. for testing
        compare=False # 'method' == 'method' regardless of location
    )

    def at(self, location: Location):
        self.location = location
        return self

@dataclass
class WordToken(Token):
    pass

@dataclass
class DataTypeToken(Token):
    kind: DataType = None

    def __post_init__(self):
        if self.kind is None:
            self.kind = DATA_TYPE[self.spelling]

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
