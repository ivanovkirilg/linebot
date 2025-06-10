from enum import Enum, auto
from dataclasses import dataclass
from typing import List

from .tokens import KeywordKind, DataType

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
