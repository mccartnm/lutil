


class StateGraphic(QtWidgets.QGraphicsItem):
    """
    Graphical respresentation of a state
    """
    def __init__(self, state, parent=None):
        super().__init__(parent)

        self._state = state
        self._bounding_rect = None


    @property
    def state(self):
        return self._state
    

    @property
    def name(self):
        return self._state.name
    

    # -- QGraphicsItem Requirements

    def boundingRect(self):
        """
        Based on the name of the state, we handle
        """
        if self._bounding_rect:
            return self._bounding_rect

