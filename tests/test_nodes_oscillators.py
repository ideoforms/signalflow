from signalflow import Sine
from signalflow import Buffer
from . import graph
from . import process_tree

import numpy as np
import pytest

Fs = 1000
N = 1000

def test_nodes_sine(graph):
    graph.sample_rate = Fs

    a = Sine([ 10, 20 ])
    b = Buffer(2, N)
    process_tree(a, buffer=b)

    #--------------------------------------------------------------------------------
    # The output of Sine() seems to diverge from np.sin() so this precision is
    # not very high. Should find out why at some point -- probably numerical
    # precision.
    #--------------------------------------------------------------------------------
    expected = np.sin(np.arange(N) * np.pi * 2 * 10 / Fs)
    assert list(b.data[0]) == pytest.approx(expected, abs=0.0001)

    expected = np.sin(np.arange(N) * np.pi * 2 * 20 / Fs)
    assert list(b.data[1]) == pytest.approx(expected, abs=0.0001)

