from signalflow import Sine
from signalflow import Buffer
import signalflow as sf
from . import graph
from . import process_tree

import numpy as np
import pytest

Fs = 1000
N = 1024

def test_nodes_oscillators_sine(graph):
    a = Sine([ 10, 20 ])
    process_tree(a, num_frames=N)

    #--------------------------------------------------------------------------------
    # The output of Sine() seems to diverge from np.sin() so this precision is
    # not very high. Should find out why at some point -- probably numerical
    # precision.
    #--------------------------------------------------------------------------------
    expected = np.sin(np.arange(N) * np.pi * 2 * 10 / graph.sample_rate)
    assert list(a.output_buffer[0]) == pytest.approx(expected, abs=0.0001)

    expected = np.sin(np.arange(N) * np.pi * 2 * 20 / graph.sample_rate)
    assert list(a.output_buffer[1]) == pytest.approx(expected, abs=0.0001)

def test_nodes_oscillators_impulse(graph):
    graph.sample_rate = 10
    a = sf.Impulse([0, 1, 2])
    process_tree(a, num_frames=graph.sample_rate * 2)
    assert np.array_equal(a.output_buffer[0], np.concatenate((
        np.ones(1),
        np.zeros(graph.sample_rate * 2 - 1)
    )))
    assert np.array_equal(a.output_buffer[1], np.concatenate((
        np.ones(1),
        np.zeros(graph.sample_rate - 1),
        np.ones(1),
        np.zeros(graph.sample_rate - 1)
    )))
    assert np.array_equal(a.output_buffer[2], np.concatenate((
        np.ones(1),
        np.zeros(graph.sample_rate // 2 - 1),
        np.ones(1),
        np.zeros(graph.sample_rate // 2 - 1),
        np.ones(1),
        np.zeros(graph.sample_rate // 2 - 1),
        np.ones(1),
        np.zeros(graph.sample_rate // 2 - 1)
    )))