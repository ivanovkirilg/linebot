import unittest

from ..parser import Parser
from ..tokens import *
from ..syntax_tree import *


class TestParser(unittest.TestCase):
    def setUp(self) -> None:
        self.parser = Parser()

    @staticmethod
    def get_basic_method(name) -> list[Token]:
        return [
            KeywordToken('method'),
            WordToken(name),
            PunctuationToken('('),
            PunctuationToken(')'),
            PunctuationToken(';'),
        ]

    def test_parses_basic_method(self):
        tokens = self.get_basic_method('do_sth')

        synt_tree = self.parser.parse(tokens)

        expected = [
            MethodDeclaration('do_sth', ReturnSpec(DataType.VOID), [])
        ]

        self.assertEqual(synt_tree, expected)

    def test_parses_multiple_methods(self):
        tokens = ( 
            self.get_basic_method('do_sth') +
            self.get_basic_method('do_other_thing')
        )

        synt_tree = self.parser.parse(tokens)

        expected = [
            MethodDeclaration('do_sth', ReturnSpec(DataType.VOID), []),
            MethodDeclaration('do_other_thing', ReturnSpec(DataType.VOID), []),
        ]

        self.assertEqual(synt_tree, expected)

    def test_parses_method_with_params(self):
        tokens = [
            KeywordToken('method'),
            WordToken('do_sth'),
            PunctuationToken('('),
                KeywordToken('in'),
                DataTypeToken('double'),
                WordToken('param_a'),
            PunctuationToken(','),
                KeywordToken('in'),
                DataTypeToken('char'),
                WordToken('param_b'),
            PunctuationToken(','),
                KeywordToken('out'),
                DataTypeToken('string'),
                WordToken('param_c'),
            PunctuationToken(')'),
            PunctuationToken(';'),
        ]

        synt_tree = self.parser.parse(tokens)

        expected = [
            MethodDeclaration(
                'do_sth',
                ReturnSpec(DataType.VOID),
                [
                    Parameter(ParamDirection.IN, DataType.DOUBLE, 'param_a'),
                    Parameter(ParamDirection.IN, DataType.CHAR, 'param_b'),
                    Parameter(ParamDirection.OUT, DataType.STRING, 'param_c'),
                ]
            )
        ]

        self.assertEqual(synt_tree, expected)

    def test_parses_method_with_return(self):
        tokens = [
            KeywordToken('method'),
            WordToken('get_sth'),
            PunctuationToken('('),
            PunctuationToken(')'),
            PunctuationToken('->'),
            DataTypeToken('bool'),
            PunctuationToken(';'),
        ]

        synt_tree = self.parser.parse(tokens)

        expected = [
            MethodDeclaration(
                'get_sth',
                ReturnSpec(DataType.BOOL),
                []
            )
        ]

        self.assertEqual(synt_tree, expected)
