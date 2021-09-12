from PySide2 import QtWidgets, QtGui, QtCore
Qt = QtCore.Qt

from .interface import LutilEditor

class LutilMainWindow(QtWidgets.QMainWindow):
    """
    Main window for the interface
    """
    def __init__(self):
        super().__init__()
        self._init_ui()

    def _init_ui(self):
        self._main = LutilEditor()
        self.setCentralWidget(self._main)
