from signalflow import AudioGraph, AudioOut_Dummy, Buffer, SineOscillator, Line, Constant, Add, Sum
from signalflow import InsufficientBufferSizeException, AudioIOException
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
    with pytest.raises(InsufficientBufferSizeException):
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
    output = AudioOut_Dummy(num_channels=5)
    graph = AudioGraph(output_device=output, start=False)
    assert graph.num_output_channels == 5
    graph.destroy()


def test_graph_render_to_buffer(graph):
    c = Constant(3)
    graph.play(c)
    b = graph.render_to_new_buffer(num_frames=graph.sample_rate)
    assert b.num_channels == 2
    assert b.num_frames == graph.sample_rate
    assert np.all(b.data[0] == 3)
    assert np.all(b.data[1] == 0)


def test_graph_nrt_set_sample_rate():
    output = AudioOut_Dummy(sample_rate=24000)
    graph = AudioGraph(output_device=output, start=False)
    assert graph.sample_rate == 24000
    graph.destroy()


def test_graph_render_after_exception():
    """
    Test that rendering after
    """
    graph = AudioGraph(output_device="dummy")
    # Instantiate a stereo graph, and play a node with an invalid number of channels
    # An exception is thrown in the audio thread
    sines = SineOscillator([440] * 6)
    sines.play()

    # Rendering should now raise a
    with pytest.raises(AudioIOException):
        graph.render()
    graph.destroy()

def test_graph_render_subgraph_to_buffer(graph):
    a = Constant(1)
    b = Constant(2)
    c = Constant(3)
    d = Add(a, [b, c])
    e = Constant(4)
    graph.play(d)
    graph.play(e)
    buf = Buffer(2, graph.sample_rate)
    graph.render_subgraph_to_buffer(d, buf)
    assert np.all(buf.data[0] == 3)
    assert np.all(buf.data[1] == 4)
    
    buf2 = Buffer(1, graph.sample_rate)
    graph.render_subgraph_to_buffer(e, buf2)
    assert np.all(buf2.data[0] == 4)

    with pytest.raises(RuntimeError):
        graph.render_subgraph_to_buffer(e, buf)