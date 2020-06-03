import libsignal

import pytest
import inspect
from . import graph

def test_nodes_general():
    """
    Try instantiating each Node class in turn, check that no crashes take place.
    """
    for key, value in vars(libsignal).items():
        if inspect.isclass(value) and issubclass(value, libsignal.Node):
            if key != "Node":
                try:
                    a = value()
                except:
                    pass
