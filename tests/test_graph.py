from libsignal import AudioGraph, Buffer, Sine
from . import process_tree, count_zero_crossings

def test_graph():
    graph = AudioGraph()
    assert graph
    del graph

def test_graph_sample_rate():
    graph = AudioGraph()
    assert graph.sample_rate > 0
    graph.sample_rate = 1000
    assert graph.sample_rate == 1000

    buf = Buffer(1, 1000)
    sine = Sine(10)
    process_tree(sine, buf)
    assert count_zero_crossings(buf.data[0]) == 10
    
    del graph