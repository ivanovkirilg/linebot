import unittest

import parser as parser
from tokens import *
from syntax_tree import *


class TestLexer(unittest.TestCase):
    def test_basic_method(self):
        tokens = [
            KeywordToken('method', KeywordKind.METHOD),
            WordToken('do_sth'),
            PunctuationToken('(', PunctuationKind.OPEN_PAREN),
            PunctuationToken(')', PunctuationKind.CLOSE_PAREN),
            PunctuationToken(';', PunctuationKind.SEMICOLON),
        ]

        synt_tree = parser.parse(tokens)

        expected = [
            MethodDeclaration('do_sth',
                              ReturnSpec(DataType.VOID),
                              [])
        ]

        self.assertEqual(synt_tree, expected)
