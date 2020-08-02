
import re
from dataclasses import dataclass

from PySide2 import QtCore, QtGui

class _Style(object):
    """
    Class for the storing style data
    """
    def __init__(self):
        self.styles = {
            "keyword"  : self._format("#3498DB"),
            "operator" : self._format("#E74C32"),
            "brace"    : self._format("#E9E9E9"),
            "basic_types": self._format("#7DC2FB"),
            "class"    : self._format("#2DCC71", "bold"),
            "function" : self._format("#2DCC71"),
            "string"   : self._format("#F1C413"),
            "string2"  : self._format("#91770E"),
            "comment"  : self._format("#606060", "italic"),
            "this"     : self._format("#F1C413", "italic"),
            "macros"   : self._format("#7EA332"),
            "numbers"  : self._format("#A23333"),
            "constants": self._format("#6B71C4"),
            "none"     : None
        }

        self.keywords = [
            "char", "class", "const",
            "double", "enum", "explicit",
            "friend", "inline", "int",
            "long", "namespace", "operator",
            "private", "protected", "public",
            "short", "signals", "signed",
            "slots", "static", "struct",
            "template", "typedef", "typename",
            "union", "unsigned", "virtual",
            "void", "volatile", "bool",
        ]
        for i, kw in enumerate(self.keywords):
            self.keywords[i] = r"\b{}\b".format(kw)

        self.base_types = [
            "int", "float", "double", "bool",
            "uint8_t", "uint16_t", "uint32_t", "uint64_t",
            "size_t"
        ]
        for i, kw in enumerate(self.base_types):
            self.base_types[i] = r"\b{}\b".format(kw)


        self.constants = [
            'true', 'false'
        ]
        for i, kw in enumerate(self.constants):
            self.constants[i] = r"\b{}\b".format(kw)

        self.operators = [
            "=",
            # Comparison
            "==", "!=", "<", "<=", ">", ">=",
            # Arithmetic
            r"\+", "-", r"\*", "/", "//", r"\%", r"\*\*",
            # In-place
            r"\+=", "-=", r"\*=", "/=", r"\%=",
            # Bitwise
            r"\^", r"\|", r"\&", r"\~", ">>", "<<"
        ]

        self.braces = [
            r"\{", r"\}", r"\(", r"\)", r"\[", r"\]"
        ]


    @staticmethod
    def _format(color, style=''):
        """
        Build a QTextCharFormat based on the input
        
        :param color: ``str`` color (hex code or name)
        :param style: ``str`` additional text formating (if any)
        :return: ``QtFuil.QTextCharFormat``
        """
        fmt = QtGui.QTextCharFormat()
        fmt.setForeground(QtGui.QColor(color))

        if 'bold' in style:
            fmt.setFontWeight(QtGui.QFont.Bold)
        if 'italic' in style:
            fmt.setFontItalic(True)
        return fmt



class CppSyntax(QtGui.QSyntaxHighlighter):
    """
    QSyntaxHighighter that understands basic C++ syntax
    """

    @dataclass
    class SyntaxRule:
        pattern: QtCore.QRegularExpression
        format_: QtGui.QTextCharFormat


    def __init__(self, document):
        super().__init__(document)
        self._rules = []

        s = _Style()
        self.styles = s.styles

        options = (
            ('keyword', s.keywords),
            ('constants', s.constants),
            ('basic_types', s.base_types),
            ('operator', s.operators),
            ('brace', s.braces),
        )

        for type_, words in options:
            for word in words:
                self._rules.append(self.SyntaxRule(
                    QtCore.QRegularExpression(word),
                    self.styles[type_]
                ))

        # -- Edge cases

        # Classic this
        self._rules.append(self.SyntaxRule(
            QtCore.QRegularExpression(r"\bthis\b"),
            self.styles["this"]
        ))

        # class/struct definition
        class_regex = [
            r"\b(?<=class|struct)\s+[A-Za-z_]+(?=\s+(\{|\:))\b",
            r"\b(?<=public|protected)\s+?[A-Za-z_0-9]+"
        ]

        for cr in class_regex:
            self._rules.append(self.SyntaxRule(
                QtCore.QRegularExpression(cr),
                self.styles["class"]
            ))

        # Quoted text
        self._rules.append(self.SyntaxRule(
            QtCore.QRegularExpression(r"\".*\""),
            self.styles["string"]
        ))

        # Basic functions
        self._rules.append(self.SyntaxRule(
            QtCore.QRegularExpression(r"\b[A-Za-z0-9_]+(?=\()"),
            self.styles["function"]
        ))

        # Template arguments
        self._rules.append(self.SyntaxRule(
            QtCore.QRegularExpression(r"(?<=[A-Za-z])(\<)(?=[^\n]+\>)"),
            self.styles["none"]
        ))
        self._rules.append(self.SyntaxRule(
            QtCore.QRegularExpression(r"(?<=[A-Za-z0-9_])\>"),
            self.styles["none"]
        ))

        # MACRO
        self._rules.append(self.SyntaxRule(
            QtCore.QRegularExpression(r"(?<=\s)[A-Z_]{2,}"),
            self.styles["macros"]
        ))

        # Single line comment
        self._rules.append(self.SyntaxRule(
            QtCore.QRegularExpression(r"//[^\n]*"),
            self.styles["comment"]
        ))

        # -- Multiline comment
        self._comment_start = QtCore.QRegularExpression(r"/\*")
        self._comment_end = QtCore.QRegularExpression(r"\*/")


    def highlightBlock(self, text):
        """
        Execute the highlighting on the correct block
        """
        for rule in self._rules:
            match_it = rule.pattern.globalMatch(text)
            while (match_it.hasNext()):
                match = match_it.next()
                if not rule.format_:
                    fmt = QtGui.QTextCharFormat()
                else:
                    fmt = rule.format_
                self.setFormat(
                    match.capturedStart(),
                    match.capturedLength(),
                    fmt
                )

        self.setCurrentBlockState(0)

        # Handle multiline comments
        # start = 0;
        # if self.previousBlockState() != 1:
        #     start = self._comment_start.capturedStart(text)

        # while start >= 0:
        #     match = self._comment_end.match(text, start)
        #     end = match.capturedStart()
        #     length = 0

        #     if end == -1:
        #         self.setCurrentBlockState(1);
        #         length = text.length() - start
        #     else:
        #         length = end - start + match.capturedLength()

        #     self.setFormat(start, length, self.styles["comment"])
        #     start = self._comment_start.capturedStart(text[start + length:])
