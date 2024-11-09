import unittest

import parser
from parser import *


class TestParser(unittest.TestCase):
    def test_space_delimited_words(self):
        tu = 'hello' + '\t' + 'world' + '\n' + 'how' + ' ' + 'are' + '  ' + 'you'

        tokens = parser.tokenize(tu)

        expected = [
            WordToken('hello'),
            WordToken('world'),
            WordToken('how'),
            WordToken('are'),
            WordToken('you')
        ]

        self.assertEqual(tokens, expected)

    def test_space_delimited_punctuation(self):
        tu = '(' + '\t' + ')' + '\n' + ',' + ' ' + ';' + '  ' + ','

        tokens = parser.tokenize(tu)

        expected = [
            PunctuationToken('(', PunctuationKind.OPEN_PAREN),
            PunctuationToken(')', PunctuationKind.CLOSE_PAREN),
            PunctuationToken(',', PunctuationKind.COMMA),
            PunctuationToken(';', PunctuationKind.SEMICOLON),
            PunctuationToken(',', PunctuationKind.COMMA),
        ]

        self.assertEqual(tokens, expected)

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
            WordToken('hello'),
            PunctuationToken(',', PunctuationKind.COMMA),
            WordToken('world'),
            PunctuationToken(';', PunctuationKind.SEMICOLON),
            WordToken('how'),
            PunctuationToken('(', PunctuationKind.OPEN_PAREN),
            WordToken('are'),
            PunctuationToken(')', PunctuationKind.CLOSE_PAREN),
            WordToken('you')
        ]

        self.assertEqual(tokens, expected)

    def test_numbers(self):
        tu = '0 +123 -456 7.8 -9.0 2.2e-3 -3.3E+4 .5'

        tokens = parser.tokenize(tu)

        expected = [
            IntegerToken('0',        0),
            IntegerToken('+123',    +123),
            IntegerToken('-456',    -456),
            FloatToken(  '7.8',      7.8),
            FloatToken(  '-9.0',    -9.0),
            FloatToken(  '2.2e-3',   2.2e-3),
            FloatToken(  '-3.3E+4', -3.3E+4),
            FloatToken(  '.5',        .5)
        ]

        self.assertEqual(tokens, expected)
