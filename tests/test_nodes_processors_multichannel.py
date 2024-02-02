import signalflow as sf
from . import graph
import math

def test_nodes_multichannel_pan(graph):
    a = 0.5
    b = sf.StereoPanner(a, -1.0)
    graph.render_subgraph(b, reset=True)
    assert all(b.output_buffer[0] == 0.5)
    assert all(b.output_buffer[1] == 0.0)

    b.set_input("pan", 0.0)
    graph.render_subgraph(b, reset=True)
    assert all(b.output_buffer[0] == 0.5 * math.sqrt(0.5))
    assert all(b.output_buffer[1] == 0.5 * math.sqrt(0.5))

def test_nodes_multichannel_width(graph):
    a = 0.5
    b = 1.0
    c = sf.StereoWidth([a, b], 1.0)
    graph.render_subgraph(c, reset=True)
    assert all(c.output_buffer[0] == 0.5)
    assert all(c.output_buffer[1] == 1.0)

    c.set_input("stereo-width", 0.0)
    graph.render_subgraph(c, reset=True)
    assert all(c.output_buffer[0] == 0.75)
    assert all(c.output_buffer[1] == 0.75)

    c.set_input("stereo-width", -1.0)
    graph.render_subgraph(c, reset=True)
    assert all(c.output_buffer[0] == 1.0)
    assert all(c.output_buffer[1] == 0.5)
