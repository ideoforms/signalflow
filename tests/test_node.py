from signalflow import SineOscillator, AudioGraph, Line, Sum, Constant, SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS
from signalflow import NodeAlreadyPlayingException, NodeNotPlayingException
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
    assert a.output_buffer.shape == (SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS, 1024)


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

    assert a.output_buffer.shape == (SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS, graph.output_buffer_size)
    a.output_buffer[SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS - 1][-1] = 1.0
    assert a.output_buffer[SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS - 1][-1] == 1.0
    with pytest.raises(IndexError):
        a.output_buffer[SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS][graph.output_buffer_size - 1] == 1.0
    with pytest.raises(IndexError):
        a.output_buffer[SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS][graph.output_buffer_size] == 1.0


def test_node_trigger(graph):
    env = sf.ASREnvelope(0, 0, 0.01)
    graph.play(env)

    graph.render()
    assert env.output_buffer[0][0] == 1.0
    assert np.all(env.output_buffer[0][1:] < 1.0)

    graph.render()
    assert np.all(env.output_buffer[0] < 1.0)

    # --------------------------------------------------------------------------------
    # trigger via function
    # --------------------------------------------------------------------------------
    env.trigger()
    graph.render()
    assert env.output_buffer[0][0] == 1.0
    assert np.all(env.output_buffer[0][1:] < 1.0)

    # --------------------------------------------------------------------------------
    # audio-rate trigger
    # --------------------------------------------------------------------------------
    node = sf.Node()
    env.set_input("clock", node)
    graph.render()
    assert np.all(env.output_buffer[0] < 1.0)

    # --------------------------------------------------------------------------------
    # audio-rate trigger
    # --------------------------------------------------------------------------------
    node.output_buffer[0][0] = 1
    graph.render()
    assert env.output_buffer[0][0] == 1.0
    assert np.all(env.output_buffer[0][1:] < 1.0)

    # --------------------------------------------------------------------------------
    # set last_sample to 1
    # --------------------------------------------------------------------------------
    node.output_buffer[0][0] = 0
    node.output_buffer[0][-1] = 1
    graph.render()
    assert np.all(env.output_buffer[0][:-1] < 1.0)
    assert env.output_buffer[0][-1] == 1.0

    # --------------------------------------------------------------------------------
    # ensure that clock does not re-trigger when first_sample == 1 and
    # last_sample == 1
    # --------------------------------------------------------------------------------
    node.output_buffer[0][1] = 0
    node.output_buffer[0][-1] = 0
    graph.render()
    assert np.all(env.output_buffer[0] < 1.0)

def test_node_play_stop_exceptions(graph):
    node = SineOscillator(440)
    node.play()
    graph.render()
    with pytest.raises(NodeAlreadyPlayingException):
        node.play()
    node.stop()
    graph.render()
    with pytest.raises(NodeNotPlayingException):
        node.stop()

def test_node_is_playing(graph):
    node = SineOscillator(440)
    assert node.is_playing is False
    node.play()
    graph.render()
    assert node.is_playing is True
    node.stop()
    # graph.render() is needed because node disconnection only happens
    # during the render process
    graph.render()
    assert node.is_playing is False

def test_node_outputs(graph):
    node = SineOscillator(440)
    assert len(node.outputs) == 0
    node.play()
    graph.render()
    assert len(node.outputs) == 1
    node.stop()
    graph.render()
    assert len(node.outputs) == 0

def test_node_variable_inputs(graph):
    # Render a sum of multiple inputs
    constants = [1, 2, 3]
    sum = Sum(constants)
    sum.play()
    graph.render()
    assert np.all(sum.output_buffer[0] == 6)

    # Add a new input
    sum.add_input(4)
    graph.render()
    assert np.all(sum.output_buffer[0] == 10)
    assert list(sum.inputs.keys()) == ["input0", "input1", "input2", "input3"]

    # Remove an input from the middle of the list, and check the sum
    sum.remove_input(sum.inputs["input1"])
    graph.render()
    assert np.all(sum.output_buffer[0] == 8)
    assert list(sum.inputs.keys()) == ["input0", "input2", "input3"]

    # Add another new input
    sum.add_input(5)
    assert list(sum.inputs.keys()) == ["input0", "input2", "input3", "input4"]
    graph.render()
    assert np.all(sum.output_buffer[0] == 13)