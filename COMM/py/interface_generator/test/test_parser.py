import unittest

from ..parser import parse
from ..tokens import *
from ..syntax_tree import *


class TestLexer(unittest.TestCase):
    def test_basic_method(self):
        tokens = [
            KeywordToken('method'),
            WordToken('do_sth'),
            PunctuationToken('('),
            PunctuationToken(')'),
            PunctuationToken(';'),
        ]

        synt_tree = parse(tokens)

        expected = [
            MethodDeclaration('do_sth', ReturnSpec(DataType.VOID), [])
        ]

        self.assertEqual(synt_tree, expected)

    def test_method_with_params(self):
        tokens = [
            KeywordToken('method'),
            WordToken('do_sth'),
            PunctuationToken('('),
                KeywordToken('in'),
                WordToken('double'),
                WordToken('param_a'),
            PunctuationToken(','),
                KeywordToken('in'),
                WordToken('char'),
                WordToken('param_b'),
            PunctuationToken(','),
                KeywordToken('out'),
                WordToken('string'),
                WordToken('param_c'),
            PunctuationToken(')'),
            PunctuationToken(';'),
        ]

        synt_tree = parse(tokens)

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