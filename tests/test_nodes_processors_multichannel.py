import signalflow as sf
from . import graph
import math

def test_nodes_multichannel_pan(graph):
    # Test panning left
    a = 0.5
    b = sf.StereoPanner(a, -1.0)
    graph.render_subgraph(b, reset=True)
    assert all(b.output_buffer[0] == 0.5)
    assert all(b.output_buffer[1] == 0.0)

    # Test panning right
    a = 0.5
    b = sf.StereoPanner(a, 1.0)
    graph.render_subgraph(b, reset=True)
    assert all(b.output_buffer[0] == 0.0)
    assert all(b.output_buffer[1] == 0.5)

    # Test panning centre
    b.set_input("pan", 0.0)
    graph.render_subgraph(b, reset=True)
    assert all(b.output_buffer[0] == 0.5 * math.sqrt(0.5))
    assert all(b.output_buffer[1] == 0.5 * math.sqrt(0.5))

    # # Test panning values beyond range (-1.0, 1.0)
    a = 0.5
    b = sf.StereoPanner(a, -1.1)
    graph.render_subgraph(b, reset=True)
    assert all(b.output_buffer[0] == 0.5)
    assert all(b.output_buffer[1] == 0.0)

    a = 0.5 
    b = sf.StereoPanner(a, 1.1)
    graph.render_subgraph(b, reset=True)
    assert all(b.output_buffer[0] == 0.0)
    assert all(b.output_buffer[1] == 0.5)