from libsignal import Sine, Square
import numpy as np

from . import process_tree
from . import graph

def test_expansion(graph):
    a = Sine(1)
    assert a.num_output_channels == 1
    assert a.num_input_channels == 1

    a = Sine([ 0.0, 1.0 ])
    assert a.num_output_channels == 2
    assert a.num_input_channels == 2

    freq = a.inputs["frequency"]
    assert freq.name == "multiplex"
    assert freq.inputs["input0"].name == "constant"
    assert freq.inputs["input1"].name == "constant"

    process_tree(a)
    assert np.all(freq.inputs["input0"].output_buffer[0] == 0.0)
    assert np.all(freq.inputs["input1"].output_buffer[0] == 1.0)

    a = Square([ 440, 441, 442 ], [ 0.3, 0.7 ])
    assert a.num_input_channels == 3
    assert a.num_output_channels == 3
    process_tree(a)