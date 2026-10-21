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
    assert a.output_buffer.shape == (SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS, a.output_buffer_length)


def test_node_add_input(graph):
    a = SineOscillator(440)
    b = SineOscillator(440)
    with pytest.raises(RuntimeError):
        a.add_input(b)


@pytest.mark.parametrize("value", [np.float32(1.5), np.float16(1.5), np.float64(1.5), np.int32(3), np.uint8(3)])
def test_node_numpy_scalar_input(graph, value):
    # np.float32 etc don't subclass Python float, so need explicit handling
    a = SineOscillator(value)
    frequency = a.get_input("frequency")
    graph.render_subgraph(a)
    assert np.all(frequency.output_buffer[0] == float(value))

    buffer = sf.Buffer([[0.25, 0.5, 0.75]])
    b = sf.ScaleLinExp(SineOscillator(1), -1, 1, np.min(buffer.data), np.max(buffer.data))
    assert b.get_input("c").output_buffer is not None


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
    # (This tests for the gotcha solved by last_input_index in VariableInputNode,
    # in which an inputX name is erroneously reused when shrinking and enlarging
    # the list)
    sum.add_input(5)
    assert list(sum.inputs.keys()) == ["input0", "input2", "input3", "input4"]
    graph.render()
    assert np.all(sum.output_buffer[0] == 13)
def test_node_set_property_list(graph):
    index = sf.Index([1, 2, 3], 1)
    assert index.get_property("list") == [1, 2, 3]

    index.set_property("list", [4.5, 5.5, 6.5])
    assert index.get_property("list") == [4.5, 5.5, 6.5]
    index.process(1)
    assert index.output_buffer[0][0] == 5.5

    # ints and numpy arrays are converted to lists of floats
    index.set_property("list", [7, 8, 9])
    assert index.get_property("list") == [7.0, 8.0, 9.0]
    index.set_property("list", np.array([0.25, 0.5, 0.75]))
    assert index.get_property("list") == [0.25, 0.5, 0.75]

    index.set_property("list", ["a", "b"])
    assert index.get_property("list") == ["a", "b"]

    with pytest.raises(TypeError):
        index.set_property("list", [1, "a"])

def test_node_set_property_nonexistent(graph):
    index = sf.Index([1, 2, 3], 1)
    with pytest.raises(RuntimeError, match="no such property"):
        index.set_property("nonexistent", 1.0)

def test_node_index_out_of_bounds(graph):
    index = sf.Index([1, 2, 3], 5)
    graph.render_subgraph(index, num_frames=1, reset=True)
    assert index.output_buffer[0][0] == 3

    index.set_input("index", -2)
    graph.render_subgraph(index, num_frames=1, reset=True)
    assert index.output_buffer[0][0] == 1

    index.set_property("list", [])
    graph.render_subgraph(index, num_frames=1, reset=True)
    assert index.output_buffer[0][0] == 0

    index.set_property("list", ["a", "b"])
    graph.render_subgraph(index, num_frames=1, reset=True)
    assert index.output_buffer[0][0] == 0


def test_node_iterate_channels(graph):
    node = sf.ChannelArray([1, 2, 3])
    assert len(node) == 3

    channels = list(node)
    assert len(channels) == 3
    for index, channel in enumerate(channels):
        assert isinstance(channel, sf.ChannelSelect)
        graph.render_subgraph(channel, num_frames=1, reset=True)
        assert channel.output_buffer[0][0] == index + 1

    last = node[-1]
    graph.render_subgraph(last, num_frames=1, reset=True)
    assert last.output_buffer[0][0] == 3
    with pytest.raises(IndexError):
        node[3]
