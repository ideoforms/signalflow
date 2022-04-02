from signalflow import SineOscillator, SquareOscillator, ChannelMixer, ChannelArray, StereoPanner, Buffer, BufferPlayer, \
    AudioGraph, AudioOut_Dummy, SIGNALFLOW_MAX_CHANNELS
from signalflow import BiquadFilter, AllpassDelay, WaveShaper, WaveShaperBuffer, Constant, Add, AudioGraphConfig
from signalflow import InvalidChannelCountException
import numpy as np
import pytest
import math

from . import process_tree, count_zero_crossings, get_peak_frequencies
from . import DEFAULT_BUFFER_LENGTH
from . import graph


def test_expansion_mono(graph):
    a = SineOscillator(1)
    assert a.num_output_channels == 1
    assert a.num_input_channels == 1


def test_expansion_multi(graph):
    """
    When passed an array as an argument, the input is automatically converted
    into a ChannelArray and the output number of channels should be increased.
    """
    a = SineOscillator([0.0, 1.0])
    assert a.num_output_channels == 2
    assert a.num_input_channels == 2

    frequency = a.inputs["frequency"]
    assert frequency.name == "channel-array"
    assert frequency.inputs["input0"].name == "constant"
    assert frequency.inputs["input1"].name == "constant"

    process_tree(a)
    assert np.all(frequency.inputs["input0"].output_buffer[0] == 0.0)
    assert np.all(frequency.inputs["input1"].output_buffer[0] == 1.0)


def test_expansion_upmix():
    output = AudioOut_Dummy(4)
    graph = AudioGraph(output_device=output)
    a = SquareOscillator([440, 880, 1320], [0.3, 0.7])
    assert a.num_input_channels == 3
    assert a.num_output_channels == 3

    process_tree(a)
    assert np.all(a.inputs["frequency"].output_buffer[0][:256] == 440.0)
    assert np.all(a.inputs["frequency"].output_buffer[1][:256] == 880.0)
    assert np.all(a.inputs["frequency"].output_buffer[2][:256] == 1320.0)
    assert np.all(a.inputs["width"].output_buffer[0][:256] == 0.3)
    assert np.all(a.inputs["width"].output_buffer[1][:256] == 0.7)
    assert np.all(a.inputs["width"].output_buffer[2][:256] == 0.0)

    assert count_zero_crossings(a.output_buffer[0][:256]) == math.ceil(440 * 256 / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[1][:256]) == math.ceil(880 * 256 / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[2][:256]) == 0

    # --------------------------------------------------------------------------------
    # When processed through a Graph, multichannel expansion duplicates the
    # 2-channel width node to become 3-channel
    # --------------------------------------------------------------------------------
    a = SquareOscillator([440, 880, 1320], [0.3, 0.7])
    graph.play(a)
    graph.render(256)
    assert graph.node_count == 3
    assert np.all(a.inputs["frequency"].output_buffer[0][:256] == 440.0)
    assert np.all(a.inputs["frequency"].output_buffer[1][:256] == 880.0)
    assert np.all(a.inputs["frequency"].output_buffer[2][:256] == 1320.0)
    assert np.all(a.inputs["width"].output_buffer[0][:256] == 0.3)
    assert np.all(a.inputs["width"].output_buffer[1][:256] == 0.7)
    assert np.all(a.inputs["width"].output_buffer[2][:256] == 0.3)
    assert count_zero_crossings(a.output_buffer[0][:256]) == math.ceil(440 * 256 / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[1][:256]) == math.ceil(880 * 256 / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[2][:256]) == math.ceil(1320 * 256 / graph.sample_rate)

    graph.stop(a)
    graph.render(256)
    # --------------------------------------------------------------------------------
    # Only remaining note is the abstract audio output node
    # --------------------------------------------------------------------------------
    assert graph.node_count == 0


def test_expansion_many_channels(graph):
    """
    Generate 100 sine channels, mix down to mono, and confirm that
    the correct 100 frequencies are present.

    By multiplying the Sine output by 0.5, we also test that the
    Multiply nodes re-allocates the output buffers of its inputs
    (the Constant(0.5)) for automatic upmixing by the AudioGraph.
    """
    frequencies = 1000 + np.arange(100) * 100
    a = SineOscillator(frequencies) * 0.5
    mixer = ChannelMixer(1, a)
    graph.render_subgraph(mixer, 2048)
    peak_frequencies = get_peak_frequencies(mixer.output_buffer[0], graph.sample_rate)
    peak_frequencies_rounded = np.round(peak_frequencies, -2)
    assert np.array_equal(peak_frequencies_rounded, frequencies)
    assert a.num_output_channels_allocated == 100
    assert a.output_buffer.shape == (100, 2048)


def test_expansion_channel_array(graph):
    a = SineOscillator(440)
    b = SineOscillator(880)
    c = SineOscillator([1760, 3520])
    d = ChannelArray([a, b, c])
    e = ChannelMixer(1, d)
    assert a.num_input_channels == a.num_output_channels == 1
    assert b.num_input_channels == b.num_output_channels == 1
    assert c.num_input_channels == c.num_output_channels == 2
    assert d.num_input_channels == d.num_output_channels == 4
    assert e.num_input_channels == 4
    assert e.num_output_channels == 1
    buf = Buffer(1, DEFAULT_BUFFER_LENGTH)
    process_tree(e, buffer=buf)
    peak_frequencies = get_peak_frequencies(buf.data[0], graph.sample_rate)
    assert np.all(
        peak_frequencies == pytest.approx([440, 880, 1760, 3520], abs=(graph.sample_rate / DEFAULT_BUFFER_LENGTH / 2)))


def test_expansion_channel_mismatch(graph):
    a = SineOscillator([440, 880])
    with pytest.raises(InvalidChannelCountException):
        b = StereoPanner(a)
    b = Buffer([1, 2, 3])
    c = BufferPlayer(b)
    with pytest.raises(InvalidChannelCountException):
        c.set_input("rate", [1, 1.5])


def test_expansion_recursive(graph):
    """
    Check that num_output_channels values propagate through the graph.
    """
    a = SineOscillator(440)
    b = BiquadFilter(a)
    buf = WaveShaperBuffer(256)
    buf.fill(lambda n: n ** 2)
    c = WaveShaper(b, buf)
    d = AllpassDelay(c)
    assert a.num_input_channels == a.num_output_channels == 1
    assert b.num_input_channels == b.num_output_channels == 1
    assert c.num_input_channels == c.num_output_channels == 1
    assert d.num_input_channels == d.num_output_channels == 1
    a.set_input("frequency", [440, 880, 1320])
    assert a.num_input_channels == a.num_output_channels == 3
    assert b.num_input_channels == b.num_output_channels == 3
    assert c.num_input_channels == c.num_output_channels == 3
    assert d.num_input_channels == d.num_output_channels == 3

    # --------------------------------------------------------------------------------
    # Note that, since 2022-03, match_input_channels no longer reduces the number
    # of channels. See comments in node.cpp -> update_channels.
    # --------------------------------------------------------------------------------
    a.set_input("frequency", 100)
    assert a.num_input_channels == a.num_output_channels == 3
    assert b.num_input_channels == b.num_output_channels == 3
    assert c.num_input_channels == c.num_output_channels == 3
    assert d.num_input_channels == d.num_output_channels == 3


def test_expansion_recursive_processing():
    """
    Check that processing/upmixing propagates through the graph when
    an input's channel count is increased.

    Needs to be run through AudioGraph because simple process_tree
    doesn't do upmixing.
    """
    output = AudioOut_Dummy(2)
    graph = AudioGraph(output_device=output)

    a = Constant(4)
    b = a + 1
    c = b * 2
    d = c - 1
    buf = Buffer(2, 1024)
    graph.play(d)
    graph.render_to_buffer(buf)
    assert np.all(buf.data[0] == 9)

    buf = Buffer(2, 1024)
    b.set_input("input0", [5, 6])
    graph.render_to_buffer(buf)
    assert np.all(buf.data[0] == 11)
    assert np.all(buf.data[1] == 13)
    graph.stop(d)


def test_expansion_buffer_reallocation(graph):
    a = SineOscillator([440] * 4)
    assert a.num_output_channels == 4
    assert a.num_output_channels_allocated == SIGNALFLOW_MAX_CHANNELS
    a.set_input("frequency", [440] * 100)
    assert a.num_output_channels == 100
    assert a.num_output_channels_allocated == 100


def test_expansion_input_reallocation(graph):
    """
    Need to allocate more output buffers for upmixing
    rename num_output_channels_allocated to num_allocated_output_buffers
    """
    channel_count = SIGNALFLOW_MAX_CHANNELS + 1
    a = Constant(4)
    b = Add(a, [9] * channel_count)
    assert b.num_output_channels == channel_count
    assert b.num_input_channels == channel_count
    assert a.num_output_channels == 1
    assert a.num_output_channels_allocated == channel_count
