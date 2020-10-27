from signalflow import Sine, AudioGraph, Line
import signalflow as sf
from . import process_tree, graph
import pytest

def test_node_no_graph():
    with pytest.raises(Exception):
        a = Sine(440)

def test_node_process(graph):
    a = Sine(440)
    a.process(1024)
    assert a.output_buffer.shape == (32, 1024)

def test_node_add_input(graph):
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

def test_node_buffer(graph):
    buf_in = sf.Buffer(([0.0] * 99) + ([1.0]))
    player = sf.BufferPlayer(buf_in, loop=False)
    graph.render_subgraph(player, 100)
    assert player.output_buffer[0][0] == 0.0
    assert player.output_buffer[0][99] == 1.0
    graph.reset_subgraph(player)
    graph.render_subgraph(player, 100)
    assert player.output_buffer[0][0] == 0.0
    assert player.output_buffer[0][99] == 0.0

def test_node_write_to_output_buffer(graph):
    """
    Test that the Python layer can successfully write to the Node's output
    buffer.
    """
    a = sf.Node()
    graph.render_subgraph(a)
    assert a.output_buffer[0][3] == 0.0
    a.output_buffer[0][3] = 1.0
    assert a.output_buffer[0][3] == 1.0

    #--------------------------------------------------------------------------------
    # Why is the output buffer of length 256 (SIGNALFLOW_DEFAULT_BLOCK_SIZE)
    # rather than 2048 (SIGNALFLOW_NODE_BUFFER_SIZE)? Because the output buffer's
    # length is reported by the Python bindings as `last_num_frames`.
    # Whether this is a good idea is open to debate.
    #
    # Better would be to have a precise and rigorous block size throughout, which
    # would mean adding a block buffer between the audio I/O and the Graph.
    #--------------------------------------------------------------------------------
    assert a.output_buffer.shape == (32, 256)
    a.output_buffer[31][255] = 1.0
    assert a.output_buffer[31][255] == 1.0
    with pytest.raises(IndexError):
        a.output_buffer[32][255] == 1.0
    with pytest.raises(IndexError):
        a.output_buffer[31][256] == 1.0