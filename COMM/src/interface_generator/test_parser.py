import unittest

import parser

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

