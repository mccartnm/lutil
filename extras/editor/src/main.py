"""
Toolkit for building well-defined, modular state machines that can help
dictate an embedded system's proceedure.
"""
import sys
from PySide2 import QtWidgets, QtGui, QtCore
Qt = QtCore.Qt

from lib.style import Colors
from lib.widgets.window import LutilMainWindow

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
    app.exec_()
