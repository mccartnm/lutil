"""
General utilities for the app
"""

class Singleton(type):
    """
    Singleton metatype for the global event manager
    """
    def __init__(cls, name, bases, dct):
        type.__init__(cls, name, bases, dct)

        _new = cls.__new__
        def singleton_new(cls, *args, **kwargs):
            if cls._singleton_instance == None:
                cls._singleton_instance = _new(cls, *args, **kwargs)
            return cls._singleton_instance

        cls._singleton_instance = None
        cls.__new__ = staticmethod(singleton_new)
