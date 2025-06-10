from .tokens import *
from .syntax_tree import *
from . import diag

class ParseError(Exception):
    pass

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
            case _:
                diag.failed()
