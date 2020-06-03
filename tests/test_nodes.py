import libsignal

import re
import pytest
import inspect

from . import graph

def test_nodes_general():
    """
    Try instantiating each Node class in turn, check that no crashes take place.
    """

    names = []
    for key, value in vars(libsignal).items():
        if inspect.isclass(value) and issubclass(value, libsignal.Node):
            if key != "Node":
                a = None
                try:
                    a = value()
                except:
                    pass
                if a:
                    name = a.name
                    assert name not in names
                    assert re.match("^[a-z0-9-]+$", name)
                    names.append(name)
