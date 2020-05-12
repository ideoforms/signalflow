from libsignal import Impulse, CombDelay, Buffer
from . import graph
from . import process_tree

import numpy as np
import pytest

def test_delay(graph):
    graph.sample_rate = 1000

    # feedback = 0
    # Input signal is passed through unmodified
    i = Impulse(0)
    a = CombDelay(i, 0.1, 0.0)
    b = Buffer(1, 1000)
    process_tree(a, buffer=b)
    assert b.data[0][0] == 1.0
    assert np.all(b.data[0][1:] == 0.0)

    # feedback = 0.5
    # Input signal is passed through with 50% feedback
    i = Impulse(0)
    a = CombDelay(i, 0.1, 0.5)
    b = Buffer(1, 1000)
    process_tree(a, buffer=b)
    assert b.data[0][0] == 1.0
    assert np.all(b.data[0][1:100] == 0.0)
    assert b.data[0][100] == 0.5
    assert np.all(b.data[0][101:200] == 0.0)
    assert b.data[0][200] == 0.25