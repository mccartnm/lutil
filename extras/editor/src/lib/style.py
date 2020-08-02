
from .utils import Singleton

from PySide2 import QtGui

class Sizes:
    """ Generic Size utility """

    # -- Text Related
    HeaderFontSize = 9
    HandleFontSize = 6
    ContainerFontSize = 10

    # --
    EditorLineNumberPadding = 8
    EditorLineNumberRightBuffer = 2



class Colors(object, metaclass=Singleton):
    """
    App-wide colors
    """
    def __init__(self):
        if hasattr(self, '_initalized'):
            return
        self._initalized = True

        self.Background = QtGui.QColor(20, 20, 20)
        self.TextBackground = QtGui.QColor(15, 15, 15)
