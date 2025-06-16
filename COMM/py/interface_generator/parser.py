from .tokens import *
from .syntax_tree import *
from .diag import eprint, highlight_error

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

    def diagnose_statement(self, tokens: list[Token]):
        match tokens:
            case [
                KeywordToken('method'),
                WordToken() as name,
                PunctuationToken('('),
                *rest
                ]:
                self.diagnose_unterminated_method(name, *rest)
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
                    if all(map(is_param_token, param_tokens)):
                        methods.append(
                            self.construct_method(name, param_tokens, ReturnSpec(DataType.VOID))
                        )
                    else:
                        self.diagnose_statement(statement)

                case [
                    KeywordToken('method'),
                    WordToken() as name,
                    PunctuationToken('('),
                    *param_tokens,
                    PunctuationToken(')'),

                    PunctuationToken('->'),
                    DataTypeToken() as return_type
                ]:
                    # TODO diagnostics on KeyError
                    return_spec = ReturnSpec(DATA_TYPE[return_type.spelling])

                    methods.append(
                        self.construct_method(name, param_tokens, return_spec)
                    )

                case _:
                    self.diagnose_statement(statement)
                    raise ParseError("Invalid structure in statement: "
                                     + ' '.join([tok.spelling for tok in statement]))

        return methods

