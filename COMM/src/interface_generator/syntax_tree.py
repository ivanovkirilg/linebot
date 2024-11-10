from enum import Enum, auto
from dataclasses import dataclass
from typing import List


class DataType(Enum):
    VOID = auto()

    BYTE = auto()
    BOOL = auto()

    INT = auto()
    FLOAT = auto()
    DOUBLE = auto()

    CHAR = auto()
    STRING = auto()

@dataclass
class ReturnSpec:
    data_type: DataType
    name: str | None = None

@dataclass
class Parameter:
    data_type: DataType
    name: str

@dataclass
class MethodDeclaration:
    name: str
    return_spec: ReturnSpec
    parameters: List[Parameter]
