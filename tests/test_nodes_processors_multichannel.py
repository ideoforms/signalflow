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