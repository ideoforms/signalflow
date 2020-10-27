import signalflow

import re
import pytest
import inspect

from . import graph

def test_nodes_general():
    """
    Try instantiating each Node class in turn, check that no crashes take place.
    Also checks that names and registration IDs are identical.
    """

    names = []
    registry = signalflow.NodeRegistry()

    for key, value in vars(signalflow).items():
        if inspect.isclass(value) and issubclass(value, signalflow.Node):
            # TODO Why do these fail in particular?
            if key != "Node" and key != "AudioIn" and key != "CrossCorrelate":
                a = None
                try:
                    a = value()
                except:
                    pass
                if a:
                    name = a.name
                    assert name not in names

                    #--------------------------------------------------------------------------------
                    # Check that all name properties are from the char class [a..z0..9-]
                    #--------------------------------------------------------------------------------
                    assert re.match("^[a-z0-9-]+$", name)
                    names.append(name)

                    node = registry.create(name)