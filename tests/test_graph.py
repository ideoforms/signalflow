from libsignal import AudioGraph, Buffer, Sine, Line
from . import process_tree, count_zero_crossings
import pytest
import numpy as np

def test_graph():
    graph = AudioGraph()
    assert graph
    del graph

def test_graph_sample_rate():
    graph = AudioGraph()
    assert graph.sample_rate > 0
    graph.sample_rate = 1000
    assert graph.sample_rate == 1000

    with pytest.raises(Exception):
        graph.sample_rate = 0

    buf = Buffer(1, 1000)
    sine = Sine(10)
    process_tree(sine, buf)
    assert count_zero_crossings(buf.data[0]) == 10

    del graph

"""
def test_graph_cyclic():
    graph = AudioGraph()
    graph.sample_rate = 1000
    line = Line(0, 1, 1)
    m1 = line * 1
    m2 = line * 2
    add = m1 + m2
    buf = Buffer(1, 1000)
    process_tree(add, buf)
    assert np.array_equal(buf.data[0], np.linspace(0, 3, graph.sample_rate))
"""