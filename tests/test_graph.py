from signalflow import AudioGraph, AudioOut_Dummy, Buffer, SineOscillator, Line, Constant, Add
from . import count_zero_crossings, graph
import pytest
import numpy as np


def test_graph_sample_rate(graph):
    assert graph.sample_rate > 0
    graph.sample_rate = 1000
    assert graph.sample_rate == 1000

    with pytest.raises(Exception):
        graph.sample_rate = 0

    buf = Buffer(1, 1000)
    sine = SineOscillator(10)
    graph.play(sine)
    graph.render_to_buffer(buf)
    assert count_zero_crossings(buf.data[0]) == 10

    del graph


def test_graph_cyclic(graph):
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


def test_graph_render(graph):
    sine = SineOscillator(440)
    graph.play(sine)
    with pytest.raises(RuntimeError):
        graph.render(441000)
    del graph


def test_graph_add_remove_node(graph):
    constant1 = Constant(1)
    constant2 = Constant(2)
    add = Add(constant1, constant2)
    graph.add_node(add)
    graph.render()
    assert np.all(graph.output.output_buffer[0] == 0.0)
    assert np.all(add.output_buffer[0] == 3)
    graph.remove_node(add)

    constant1 = Constant(1)
    constant2 = Constant(2)
    add = Add(constant1, constant2)
    graph.add_node(add)
    graph.remove_node(add)
    graph.render()
    assert np.all(graph.output.output_buffer[0] == 0.0)
    assert np.all(add.output_buffer[0] == 0)
    del graph


def test_graph_clear(graph):
    c1 = Constant(1)
    graph.play(c1)
    c2 = Constant(2)
    graph.play(c2)
    buffer = Buffer(1, 1024)
    graph.render_to_buffer(buffer)
    assert np.all(buffer.data[0] == 3)
    graph.clear()
    graph.render_to_buffer(buffer)
    assert np.all(buffer.data[0] == 0)


def test_graph_num_output_channels():
    output = AudioOut_Dummy(5)
    graph = AudioGraph(output_device=output, start=False)
    assert graph.num_output_channels == 5
    del graph


def test_graph_render_to_buffer(graph):
    c = Constant(3)
    graph.play(c)
    b = graph.render_to_new_buffer(num_frames=graph.sample_rate)
    assert b.num_channels == 2
    assert b.num_frames == graph.sample_rate
    assert np.all(b.data[0] == 3)
    assert np.all(b.data[1] == 0)
