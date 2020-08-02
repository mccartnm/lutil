"""
Toolkit for building well-defined, modular state machines that can help
dictate an embedded system's proceedure.
"""
import sys
from PySide2 import QtWidgets, QtGui, QtCore
Qt = QtCore.Qt

from lib.cpp.cpp_syntax import CppSyntax
from lib.cpp.cpp_textedit import CppTextEdit
from lib.style import Colors

TEXT = """\
template<typename T> // ... TODO (get all this highlighting)
class Foo : public StateDriver<Foo> {

public:
    Foo() {}

    void engine_run() {
        // TODO
    }

    bool turn_engine_on() {
        // TODO
    }

    REGISTER_RUNTIMES(
        engine_run
    );

    REGISTER_TRANSITIONS(
        turn_engine_on
    );

    void Ref_DDD() {
    }
};

"""

class LutilMainWindow(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()

        self._init_ui()


    def _init_ui(self):
        # layout = QtWidgets.QVBoxLayout()
        # layout.setContentsMargins(2, 2, 2, 2)

        self._text_edit = CppTextEdit()
        self._text_edit.setReadOnly(False)
        self._syntax = CppSyntax(self._text_edit.document())

        # layout.addWidget(self._text_edit)
        self.setCentralWidget(self._text_edit)


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    app.setStyle('Fusion')

    win = LutilMainWindow()
    win.show()
    win.resize(450, 450)

    QPalette = QtGui.QPalette
    QColor = QtGui.QColor
    colors = Colors()

    dark_palette = QPalette()
    dark_palette.setColor(QPalette.Window, colors.Background)
    dark_palette.setColor(QPalette.WindowText, Qt.white)
    dark_palette.setColor(QPalette.Base, colors.TextBackground)
    dark_palette.setColor(QPalette.AlternateBase, colors.Background)
    dark_palette.setColor(QPalette.ToolTipBase, Qt.white)
    dark_palette.setColor(QPalette.ToolTipText, Qt.white)
    dark_palette.setColor(QPalette.Text, Qt.white)
    dark_palette.setColor(QPalette.Button, QColor(53, 53, 53))
    dark_palette.setColor(QPalette.ButtonText, Qt.white)
    dark_palette.setColor(QPalette.BrightText, Qt.red)
    dark_palette.setColor(QPalette.Link, QColor(42, 130, 218))
    dark_palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
    dark_palette.setColor(QPalette.HighlightedText, Qt.white)

    app.setPalette(dark_palette)

    win._text_edit.setPlainText(TEXT)

    app.exec_()
