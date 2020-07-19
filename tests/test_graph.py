from signalflow import AudioGraph, Buffer, Sine, Line
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

def test_graph_cyclic():
    graph = AudioGraph()
    graph.sample_rate = 1000
    line = Line(0, 1, 1)
    m1 = line * 1
    m2 = line * 2
    add = m1 + m2
    graph.play(add)
    buf = Buffer(1, 1000)
    graph.render_to_buffer(buf)
    assert np.all(np.abs(buf.data[0] - np.linspace(0, 3, graph.sample_rate)) < 0.00001)
    del graph

def test_graph_render():
    graph = AudioGraph()
    sine = Sine(440)
    graph.play(sine)
    with pytest.raises(RuntimeError):
        graph.render(441000)
    del graph