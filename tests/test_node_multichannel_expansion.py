from libsignal import Sine, Square
import numpy as np

from . import process_tree, count_zero_crossings
from . import graph
from . import DEFAULT_BUFFER_LENGTH

def test_expansion_mono(graph):
    a = Sine(1)
    assert a.num_output_channels == 1
    assert a.num_input_channels == 1

def test_expansion_multi(graph):
    a = Sine([ 0.0, 1.0 ])
    assert a.num_output_channels == 2
    assert a.num_input_channels == 2

    frequency = a.inputs["frequency"]
    assert frequency.name == "multiplex"
    assert frequency.inputs["input0"].name == "constant"
    assert frequency.inputs["input1"].name == "constant"

    process_tree(a)
    assert np.all(frequency.inputs["input0"].output_buffer[0] == 0.0)
    assert np.all(frequency.inputs["input1"].output_buffer[0] == 1.0)

    a = Square([ 440, 880, 1320 ], [ 0.3, 0.7 ])
    assert a.num_input_channels == 3
    assert a.num_output_channels == 3

    process_tree(a)
    assert np.all(a.inputs["frequency"].output_buffer[0] == 440.0)
    assert np.all(a.inputs["frequency"].output_buffer[1] == 880.0)
    assert np.all(a.inputs["frequency"].output_buffer[2] == 1320.0)
    assert np.all(a.inputs["width"].output_buffer[0] == 0.3)
    assert np.all(a.inputs["width"].output_buffer[1] == 0.7)
    assert np.all(a.inputs["width"].output_buffer[2] == 0.0)

    assert count_zero_crossings(a.output_buffer[0]) == int(440 * DEFAULT_BUFFER_LENGTH / graph.sample_rate + 0.5)
    assert count_zero_crossings(a.output_buffer[1]) == int(880 * DEFAULT_BUFFER_LENGTH / graph.sample_rate + 0.5)
    assert count_zero_crossings(a.output_buffer[2]) == 0

    #--------------------------------------------------------------------------------
    # When processed through a Graph, multichannel expansion duplicates the
    # 2-channel width node to become 3-channel
    #--------------------------------------------------------------------------------
    a = Square([ 440, 880, 1320 ], [ 0.3, 0.7 ])
    graph.add_output(a)
    graph.pull_input(1024)
    assert graph.node_count == 9
    assert np.all(a.inputs["frequency"].output_buffer[0] == 440.0)
    assert np.all(a.inputs["frequency"].output_buffer[1] == 880.0)
    assert np.all(a.inputs["frequency"].output_buffer[2] == 1320.0)
    assert np.all(a.inputs["width"].output_buffer[0] == 0.3)
    assert np.all(a.inputs["width"].output_buffer[1] == 0.7)
    assert np.all(a.inputs["width"].output_buffer[2] == 0.3)
    assert count_zero_crossings(a.output_buffer[0]) == int(440 * DEFAULT_BUFFER_LENGTH / graph.sample_rate + 0.5)
    assert count_zero_crossings(a.output_buffer[1]) == int(880 * DEFAULT_BUFFER_LENGTH / graph.sample_rate + 0.5)
    assert count_zero_crossings(a.output_buffer[2]) == int(1320 * DEFAULT_BUFFER_LENGTH / graph.sample_rate + 0.5)

    graph.remove_output(a)
    graph.pull_input(1024)
    #--------------------------------------------------------------------------------
    # Only remaining note is the abstract audio output node
    #--------------------------------------------------------------------------------
    assert graph.node_count == 1