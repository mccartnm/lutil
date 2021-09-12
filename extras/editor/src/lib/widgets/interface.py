"""
Base interface widget for the state machine tooling
"""
from PySide2 import QtWidgets, QtCore

from ..cpp.cpp_syntax import CppSyntax
from ..cpp.cpp_textedit import CppTextEdit

class LutilEditor(QtWidgets.QWidget):
    """
    State machine interface
    """
    def __init__(self, parent=None):
        super().__init__(parent)

        self._init_ui()


    def _init_ui(self):
        """
        Construct a simple, scalable interface for the toolkit
        """
        # Transparent setup
        layout = QtWidgets.QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)

        self._splitter = QtWidgets.QSplitter()

        self._text_edit = CppTextEdit()
        self._text_edit.setReadOnly(False)
        self._syntax = CppSyntax(self._text_edit.document())

        self._splitter.addWidget(self._text_edit)

        layout.addWidget(self._splitter)
        self.setLayout(layout)
