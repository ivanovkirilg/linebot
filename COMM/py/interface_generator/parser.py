from typing import Iterable
from .tokens import *
from .syntax_tree import *
from . import diag

import difflib

SIMILARITY_THRESHOLD = 0.7

class ParseError(Exception):
    pass

def up_to(iter, delimiter):
    for elem in iter:
        if elem == delimiter:
            return
        yield elem

def until(iter, condition):
    for elem in iter:
        if condition(elem):
            return
        yield elem

def is_param_token(token: Token) -> bool:
    match token:
        case KeywordToken('in') | KeywordToken('out') | DataTypeToken() | WordToken() | PunctuationToken(','):
            return True
        case _:
            return False

class Parser:
    def __init__(self, source: list[str] = []):
        self._source = source

    def separate(self, tokens: list[Token], delimiter: Token) -> list[list[Token]]:
        sections = []
        start = end = 0
        while start is not None:
            try:
                end = tokens.index(delimiter, start)
                sections.append(tokens[start:end])
                start = end + 1
            except ValueError:
                if start != len(tokens):
                    sections.append(tokens[start:])
                start = None
        return sections

    def parse_parameters(self, parameter_tokens: list[Token], out_params: list[Parameter]):
        parameters = self.separate(parameter_tokens, PunctuationToken(','))
        for parameter in parameters:
            match parameter:
                case []:
                    return
                case [
                        KeywordToken() as direction,
                        DataTypeToken() as data_type,
                        WordToken() as name,
                    ]:
                    out_params.append(
                        Parameter(
                            PARAM_DIRECTION[direction.kind],
                            DATA_TYPE[data_type.spelling],
                            name.spelling)
                    )
                case _:
                    self.diagnose_parameter(parameter)
                    raise ParseError(f"Invalid parameters structure {parameter}")

    def construct_method(self, name: WordToken,
                         param_tokens: list[Token],
                         return_spec: ReturnSpec) -> MethodDeclaration:
        parameters = []
        self.parse_parameters(param_tokens, parameters)

        return MethodDeclaration(
            name.spelling,
            return_spec,
            parameters
        )

    def parse_method(self,
                     statement: list[Token],
                     name: WordToken,
                     param_tokens: list[Token],
                     return_type: DataTypeToken | None = None):
        return_spec = ReturnSpec(
            (DATA_TYPE[return_type.spelling] if return_type else DataType.VOID)
        )

        if all(map(is_param_token, param_tokens)):
            return self.construct_method(name, param_tokens, return_spec)
        else:
            self.diagnose_statement(statement)
            raise ParseError('Invalid method declaration')

    def parse(self, tokens: list[Token]) -> list[MethodDeclaration]:
        statements = self.separate(tokens, PunctuationToken(';'))

        methods = []

        for statement in statements:
            match statement:
                case [
                        KeywordToken('method'),
                        WordToken() as name,
                        PunctuationToken('('),
                        *param_tokens,
                        PunctuationToken(')')
                    ]:
                    methods.append(
                        self.parse_method(statement, name, param_tokens)
                    )
                case [
                    KeywordToken('method'),
                    WordToken() as name,
                    PunctuationToken('('),
                    *param_tokens,
                    PunctuationToken(')'),

                    PunctuationToken('->'),
                    DataTypeToken() as return_type
                ]:
                    methods.append(
                        self.parse_method(statement, name, param_tokens, return_type)
                    )

                case _:
                    self.diagnose_statement(statement)
                    raise ParseError("Invalid structure in statement: "
                                     + ' '.join([tok.spelling for tok in statement]))

        return methods

    def highlight(self, token: Token, last: Token | None = None, between = False):
        start = (token.location.column_range[0]
                 if not between
                 else token.location.column_range[1])
        end = (token.location.column_range[1]
               if last is None else
               last.location.column_range[1]
               if not between else
               last.location.column_range[0])

        diag.highlight_error(self._source[token.location.line_nr - 1], start, end)

    def diagnose_misspelling(self, invalid_token: Token, possibilities: Iterable[str], invalid_message: str):
        maybe_misspelling = difflib.get_close_matches(
            invalid_token.spelling, possibilities, n=1, cutoff=SIMILARITY_THRESHOLD)

        if maybe_misspelling:
            diag.eprint(invalid_message)
            diag.eprint(f"Did you mean '{maybe_misspelling[0]}'?")
            self.highlight(invalid_token)
        else:
            diag.failed()

    def diagnose_parameter(self, tokens: list[Token]):
        diag.error('Invalid parameter', tokens[0].location.line_nr)
        self.highlight(tokens[0], tokens[-1])

        match tokens:
            case [
                DataTypeToken() as data_type,
                WordToken() as name
                ]:
                diag.eprint(f"Missing direction 'in' or 'out' here:")
                self.highlight(data_type, name, between=True)
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                DataTypeToken() as data_type
                ]:
                diag.eprint(f"Missing parameter name here:")
                self.highlight(direction, data_type, between=True)
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                WordToken() as name
                ]:
                diag.eprint(f"Missing data type here:")
                self.highlight(direction, name, between=True)
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                WordToken() as not_data_type,
                WordToken() as name
                ]:
                self.diagnose_misspelling(not_data_type, DATA_TYPE.keys(), f"Invalid data type")
            case [
                Token() as not_direction,
                DataTypeToken() as data_type,
                WordToken() as name
                ]:
                self.diagnose_misspelling(not_direction, ['in', 'out'], f"Invalid direction")

            case _:
                diag.failed()

    def diagnose_unterminated_method(self, name_tok: Token, *rest: Token):
        unterminated = list(up_to(rest, KeywordToken('method')))
        if len(unterminated) < len(rest) or unterminated[-1] == PunctuationToken(')'):
            match unterminated:
                case [
                    *params,
                    PunctuationToken(')'),
                    PunctuationToken('->'),
                    DataTypeToken() as last
                    ] | [
                    *params,
                    PunctuationToken(')') as last
                    ]:
                    diag.eprint(f"Statement not terminated, missing ';' here:")
                    loc = last.location
                    diag.highlight_error(self._source[loc.line_nr - 1], loc.column_range[1], loc.column_range[1] + 1)
                case _:
                    diag.failed()
        else:
            diag.failed()

    def diagnose_statement(self, tokens: list[Token]):
        diag.error('Invalid statement', tokens[0].location.line_nr)
        diag.eprint(self._source[tokens[0].location.line_nr - 1])

        match tokens:
            case [
                KeywordToken('method'),
                WordToken() as name,
                PunctuationToken('('),
                *param_tokens,
                PunctuationToken(')'),
                PunctuationToken('->'),
                WordToken() as not_return_type
                ]:
                self.diagnose_misspelling(not_return_type, DATA_TYPE.keys(), f"Invalid return type")

            case [
                KeywordToken('method'),
                WordToken() as name,
                PunctuationToken('('),
                *rest
                ]:
                self.diagnose_unterminated_method(name, *rest)
            case [
                WordToken() as not_method,
                WordToken() as name,
                PunctuationToken('('),
                *params,
                PunctuationToken(')')
                ]:
                self.diagnose_misspelling(not_method, ['method'], f"Invalid statement")

            case _:
                diag.failed()
