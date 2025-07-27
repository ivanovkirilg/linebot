from .tokens import *
from .syntax_tree import *
from .diag import eprint, highlight_error
from difflib import SequenceMatcher, get_close_matches

SIMILARITY_THRESHOLD = 0.7

class ParseError(Exception):
    pass

def diag_failed():
    eprint(">>>> DIAGNOSTICS FAILED <<<<")

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

    def diagnose_parameter(self, tokens: list[Token]):
        match tokens:
            case [
                DataTypeToken() as data_type,
                WordToken() as name
                ]:
                eprint(f"Missing direction ('in' or 'out') for parameter '{data_type.spelling} {name.spelling}'")
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                DataTypeToken() as data_type
                ]:
                eprint(f"Missing parameter name for '{direction.spelling} {data_type.spelling}'")
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                WordToken() as name
                ]:
                eprint(f"Missing data type for parameter '{direction.spelling} {name.spelling}'")
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                WordToken() as not_data_type,
                WordToken() as name
                ]:
                maybe_data_type = get_close_matches(not_data_type.spelling, DATA_TYPE.keys(), n=1, cutoff=SIMILARITY_THRESHOLD)
                if maybe_data_type:
                    eprint(f"Invalid data type for parameter '{name.spelling}'")
                    eprint(f"Did you mean '{maybe_data_type[0]}'?")
                else:
                    diag_failed()
            case [
                Token() as not_direction,
                DataTypeToken() as data_type,
                WordToken() as name
                ]:
                maybe_direction = get_close_matches(not_direction.spelling, ['in', 'out'], n=1, cutoff=SIMILARITY_THRESHOLD)
                if maybe_direction:
                    eprint(f"Invalid direction for parameter '{name.spelling}'")
                    eprint(f"Did you mean '{maybe_direction[0]}'?")
                else:
                    diag_failed()
            case _:
                diag_failed()
        highlight_error(self._source[tokens[0].location.line_nr - 1], tokens[0].location.column_range[0], tokens[-1].location.column_range[1])

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
                    eprint(f"Method declaration for '{name_tok.spelling}' missing ';' (not terminated):")
                    loc = last.location
                    highlight_error(self._source[loc.line_nr - 1], loc.column_range[1], loc.column_range[1] + 1)
                case _:
                    diag_failed()
        else:
            diag_failed()

    def diagnose_misspelled_method(self, not_method: WordToken):
        eprint("Invalid statement: did you mean 'method'?")
        highlight_error(self._source[not_method.location.line_nr - 1], *not_method.location.column_range)

    def diagnose_statement(self, tokens: list[Token]):
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
                maybe_return_type = get_close_matches(not_return_type.spelling, DATA_TYPE.keys(), n=1, cutoff=SIMILARITY_THRESHOLD)
                if maybe_return_type:
                    eprint(f"Invalid return type for method '{name.spelling}': '{not_return_type.spelling}'")
                    eprint(f"Did you mean '{maybe_return_type[0]}'?")
                else:
                    diag_failed()
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
                if SequenceMatcher(None, not_method.spelling, 'method').ratio() > SIMILARITY_THRESHOLD:
                    self.diagnose_misspelled_method(not_method)
                else:
                    diag_failed()
            case _:
                diag_failed()

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

