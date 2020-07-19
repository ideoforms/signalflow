from signalflow import Sine, AudioGraph

import pytest

def test_node_process():
    a = Sine(440)
    with pytest.raises(Exception):
        a.process(1024)

    graph = AudioGraph()
    a = Sine(440)
    a.process(1024)
    assert a.output_buffer.shape == (32, 1024)
    del graph

def test_node_add_input():
    a = Sine(440)
    b = Sine(440)
    with pytest.raises(RuntimeError):
        a.add_input(b)