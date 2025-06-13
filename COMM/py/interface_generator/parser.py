from .tokens import *
from .syntax_tree import *

class Parser:
    def __init__(self, source: list[str] = []):
        self._source = source

    def highlight_error(self, tokens: list[Token]):
        start_col = tokens[0].location.column_range[0]
        end_col = tokens[-1].location.column_range[1]
        print(self._source[tokens[0].location.line_nr - 1])
        print(' ' * start_col,
              '^' * (end_col - start_col),
              sep='')

    def diagnose_parameter(self, tokens: list[Token]):
        match tokens:
            case [
                DataTypeToken() as data_type,
                WordToken() as name
                ]:
                print(f"Missing direction ('in' or 'out') for parameter '{data_type.spelling} {name.spelling}'")
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                DataTypeToken() as data_type
                ]:
                print(f"Missing parameter name for '{direction.spelling} {data_type.spelling}'")
            case [
                KeywordToken('in') | KeywordToken('out') as direction,
                WordToken() as name
                ]:
                print(f"Missing data type for parameter '{direction.spelling} {name.spelling}'")
            case _:
                print(">>>> DIAGNOSTICS FAILED <<<<")
        self.highlight_error(tokens)

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
                    raise ValueError(f"Invalid parameters structure {parameter}")

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
                    methods.append(
                        self.construct_method(name, param_tokens, ReturnSpec(DataType.VOID))
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
                    # TODO diagnostics on KeyError
                    return_spec = ReturnSpec(DATA_TYPE[return_type.spelling])

                    methods.append(
                        self.construct_method(name, param_tokens, return_spec)
                    )

                case _:
                    raise ValueError("Invalid structure in statement: "
                                     + ' '.join([tok.spelling for tok in statement]))

        return methods

