from signalflow import Sine, AudioGraph, Line
from . import process_tree, graph
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

def test_node_set_input(graph):
    a = Sine(440)

    #--------------------------------------------------------------------------------
    # Note that, when an input is set to a scalar value and the existing input
    # node is of type Constant, the value of the existing Constant is updated.
    # This means that we can re-use the same `frequency` reference below.
    #--------------------------------------------------------------------------------
    frequency = a.get_input("frequency")
    process_tree(a, num_frames=1024)
    assert frequency.output_buffer[0][0] == 440
    a.set_input("frequency", 880)
    process_tree(a, num_frames=1024)
    assert frequency.output_buffer[0][0] == 880

    a.set_input("frequency", Line(0, 1, 1.0))
    process_tree(a, num_frames=1024)
    line = a.get_input("frequency")
    assert line.output_buffer[0][0] == 0
    assert line.output_buffer[0][1] > 0

    #--------------------------------------------------------------------------------
    # Check that replacing a non-Constant with a Constant works as expected.
    #--------------------------------------------------------------------------------
    a.set_input("frequency", 1760)
    frequency = a.get_input("frequency")
    process_tree(a, num_frames=1024)
    assert frequency.output_buffer[0][0] == 1760