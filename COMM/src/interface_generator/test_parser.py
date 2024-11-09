import unittest

import parser
from parser import Token, TokenKind, PunctuationKind

class TestParser(unittest.TestCase):
    def test_space_delimited_words(self):
        tu = 'hello' + '\t' + 'world' + '\n' + 'how' + ' ' + 'are' + '  ' + 'you'

        tokens = parser.tokenize(tu)

        spellings = [tok.spelling for tok in tokens]
        kinds = [tok.kind for tok in tokens]

        self.assertEqual(spellings, ['hello', 'world', 'how', 'are', 'you'])
        self.assertEqual(kinds, [parser.TokenKind.WORD] * 5)

    def test_space_delimited_punctuation(self):
        tu = '(' + '\t' + ')' + '\n' + ',' + ' ' + ';' + '  ' + ','

        tokens = parser.tokenize(tu)

        spellings = [tok.spelling for tok in tokens]
        kinds = [tok.kind for tok in tokens]
        punctKinds = [tok.punctKind for tok in tokens]

        self.assertEqual(spellings, ['(', ')', ',', ';', ','])
        self.assertEqual(kinds, [parser.TokenKind.PUNCTUATION] * 5)
        self.assertEqual(punctKinds, [
            parser.PunctuationKind.OPEN_PAREN,
            parser.PunctuationKind.CLOSE_PAREN,
            parser.PunctuationKind.COMMA,
            parser.PunctuationKind.SEMICOLON,
            parser.PunctuationKind.COMMA,
        ])

    def test_unsupported_punctuation(self):
        tu = 'hello!'

        with self.assertRaises(ValueError) as exc_context:
            _ = parser.tokenize(tu)

        self.assertIn('punct', str(exc_context.exception))
        self.assertIn('!', str(exc_context.exception))

    def test_punctuation_delimited_words(self):
        tu = 'hello,world;how(are)you'

        tokens = parser.tokenize(tu)

        expected = [
            Token(TokenKind.WORD,'hello'),
            Token(TokenKind.PUNCTUATION,',', punctKind=PunctuationKind.COMMA),
            Token(TokenKind.WORD,'world'),
            Token(TokenKind.PUNCTUATION,';', punctKind=PunctuationKind.SEMICOLON),
            Token(TokenKind.WORD,'how'),
            Token(TokenKind.PUNCTUATION,'(', punctKind=PunctuationKind.OPEN_PAREN),
            Token(TokenKind.WORD,'are'),
            Token(TokenKind.PUNCTUATION,')', punctKind=PunctuationKind.CLOSE_PAREN),
            Token(TokenKind.WORD,'you')
        ]

        self.assertEqual(tokens, expected)

