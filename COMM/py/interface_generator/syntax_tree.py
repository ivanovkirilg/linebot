from enum import Enum, auto
from dataclasses import dataclass
from typing import List

from .tokens import KeywordKind

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

class ParamDirection(Enum):
    IN = auto()
    OUT = auto()

PARAM_DIRECTION = {
    KeywordKind.IN: ParamDirection.IN,
    KeywordKind.OUT: ParamDirection.OUT,
}

@dataclass
class ReturnSpec:
    data_type: DataType
    name: str | None = None

@dataclass
class Parameter:
    direction: ParamDirection
    data_type: DataType
    name: str

@dataclass
class MethodDeclaration:
    name: str
    return_spec: ReturnSpec
    parameters: List[Parameter]
