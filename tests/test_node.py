from signalflow import SineOscillator, AudioGraph, Line, SIGNALFLOW_MAX_CHANNELS
import signalflow as sf
import numpy as np
from . import graph
import pytest


def test_node_no_graph():
    with pytest.raises(Exception):
        a = SineOscillator(440)


def test_node_process(graph):
    a = SineOscillator(440)
    a.process(1024)
    assert a.output_buffer.shape == (SIGNALFLOW_MAX_CHANNELS, 1024)


def test_node_add_input(graph):
    a = SineOscillator(440)
    b = SineOscillator(440)
    with pytest.raises(RuntimeError):
        a.add_input(b)


def test_node_set_input(graph):
    a = SineOscillator(440)

    # --------------------------------------------------------------------------------
    # Note that, when an input is set to a scalar value and the existing input
    # node is of type Constant, the value of the existing Constant is updated.
    # This means that we can re-use the same `frequency` reference below.
    # --------------------------------------------------------------------------------
    frequency = a.get_input("frequency")
    graph.render_subgraph(a)
    assert np.all(frequency.output_buffer[0] == 440)
    a.set_input("frequency", 880)
    graph.render_subgraph(a, reset=True)
    assert np.all(frequency.output_buffer[0] == 880)

    a.set_input("frequency", Line(0, 1, 1.0))
    graph.render_subgraph(a, reset=True)
    line = a.get_input("frequency")
    assert line.output_buffer[0][0] == 0
    assert line.output_buffer[0][1] > 0

    # --------------------------------------------------------------------------------
    # Check that replacing a non-Constant with a Constant works as expected.
    # --------------------------------------------------------------------------------
    a.set_input("frequency", 1760)
    frequency = a.get_input("frequency")
    graph.render_subgraph(a, reset=True)
    assert np.all(frequency.output_buffer[0] == 1760)


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

    # --------------------------------------------------------------------------------
    # Why is the output buffer of length 256 (SIGNALFLOW_DEFAULT_BLOCK_SIZE)
    # rather than 2048 (SIGNALFLOW_NODE_BUFFER_SIZE)? Because the output buffer's
    # length is reported by the Python bindings as `last_num_frames`.
    # Whether this is a good idea is open to debate.
    #
    # Better would be to have a precise and rigorous block size throughout, which
    # would mean adding a block buffer between the audio I/O and the Graph.
    # --------------------------------------------------------------------------------
    assert a.output_buffer.shape == (SIGNALFLOW_MAX_CHANNELS, 256)
    a.output_buffer[SIGNALFLOW_MAX_CHANNELS - 1][255] = 1.0
    assert a.output_buffer[SIGNALFLOW_MAX_CHANNELS - 1][255] == 1.0
    with pytest.raises(IndexError):
        a.output_buffer[SIGNALFLOW_MAX_CHANNELS][255] == 1.0
    with pytest.raises(IndexError):
        a.output_buffer[SIGNALFLOW_MAX_CHANNELS][256] == 1.0


def test_node_trigger(graph):
    env = sf.ASREnvelope(0, 0, 0.01)
    graph.play(env)

    graph.render(1024)
    assert env.output_buffer[0][0] == 1.0
    assert np.all(env.output_buffer[0][1:] < 1.0)

    graph.render(1024)
    assert np.all(env.output_buffer[0] < 1.0)

    # --------------------------------------------------------------------------------
    # trigger via function
    # --------------------------------------------------------------------------------
    env.trigger()
    graph.render(1024)
    assert env.output_buffer[0][0] == 1.0
    assert np.all(env.output_buffer[0][1:] < 1.0)

    # --------------------------------------------------------------------------------
    # audio-rate trigger
    # --------------------------------------------------------------------------------
    node = sf.Node()
    env.set_input("clock", node)
    graph.render(1024)
    assert np.all(env.output_buffer[0] < 1.0)

    # --------------------------------------------------------------------------------
    # audio-rate trigger
    # --------------------------------------------------------------------------------
    node.output_buffer[0][0] = 1
    graph.render(1024)
    assert env.output_buffer[0][0] == 1.0
    assert np.all(env.output_buffer[0][1:] < 1.0)

    # --------------------------------------------------------------------------------
    # set last_sample to 1
    # --------------------------------------------------------------------------------
    node.output_buffer[0][0] = 0
    node.output_buffer[0][1023] = 1
    graph.render(1024)
    assert np.all(env.output_buffer[0][:1023] < 1.0)
    assert env.output_buffer[0][1023] == 1.0

    # --------------------------------------------------------------------------------
    # ensure that clock does not re-trigger when first_sample == 1 and
    # last_sample == 1
    # --------------------------------------------------------------------------------
    node.output_buffer[0][1] = 0
    node.output_buffer[0][1023] = 0
    graph.render(1024)
    assert np.all(env.output_buffer[0] < 1.0)
