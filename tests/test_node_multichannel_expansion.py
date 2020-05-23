from libsignal import Sine, Square, Mixer, Buffer
import scipy.signal
import numpy as np
import math

from . import process_tree, count_zero_crossings
from . import DEFAULT_BUFFER_LENGTH
from . import graph

def test_expansion_mono(graph):
    a = Sine(1)
    assert a.num_output_channels == 1
    assert a.num_input_channels == 1

def test_expansion_multi(graph):
    """
    When passed an array as an argument, the input is automatically converted
    into a ChannelArray and the output number of channels should be increased.
    """
    a = Sine([ 0.0, 1.0 ])
    assert a.num_output_channels == 2
    assert a.num_input_channels == 2

    frequency = a.inputs["frequency"]
    assert frequency.name == "channel-array"
    assert frequency.inputs["input0"].name == "constant"
    assert frequency.inputs["input1"].name == "constant"

    process_tree(a)
    assert np.all(frequency.inputs["input0"].output_buffer[0] == 0.0)
    assert np.all(frequency.inputs["input1"].output_buffer[0] == 1.0)

def test_expansion_upmix(graph):
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

    assert count_zero_crossings(a.output_buffer[0]) == math.ceil(440 * DEFAULT_BUFFER_LENGTH / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[1]) == math.ceil(880 * DEFAULT_BUFFER_LENGTH / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[2]) == 0

    #--------------------------------------------------------------------------------
    # When processed through a Graph, multichannel expansion duplicates the
    # 2-channel width node to become 3-channel
    #--------------------------------------------------------------------------------
    a = Square([ 440, 880, 1320 ], [ 0.3, 0.7 ])
    graph.add_output(a)
    graph.pull_input(1024)
    assert graph.node_count == 4
    assert np.all(a.inputs["frequency"].output_buffer[0] == 440.0)
    assert np.all(a.inputs["frequency"].output_buffer[1] == 880.0)
    assert np.all(a.inputs["frequency"].output_buffer[2] == 1320.0)
    assert np.all(a.inputs["width"].output_buffer[0] == 0.3)
    assert np.all(a.inputs["width"].output_buffer[1] == 0.7)
    assert np.all(a.inputs["width"].output_buffer[2] == 0.3)
    assert count_zero_crossings(a.output_buffer[0]) == math.ceil(440 * DEFAULT_BUFFER_LENGTH / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[1]) == math.ceil(880 * DEFAULT_BUFFER_LENGTH / graph.sample_rate)
    assert count_zero_crossings(a.output_buffer[2]) == math.ceil(1320 * DEFAULT_BUFFER_LENGTH / graph.sample_rate)

    graph.remove_output(a)
    graph.pull_input(1024)
    #--------------------------------------------------------------------------------
    # Only remaining note is the abstract audio output node
    #--------------------------------------------------------------------------------
    assert graph.node_count == 1

def get_peak_frequencies(samples, sample_rate):
    magnitudes = np.abs(np.fft.rfft(samples))
    peaks, _ = scipy.signal.find_peaks(magnitudes)
    peaks = peaks * sample_rate / len(samples)
    return peaks

def test_expansion_max_channels(graph):
    frequencies = 1000 + np.arange(32) * 100
    a = Sine(frequencies)
    mixer = Mixer(1, a)
    b = Buffer(1, DEFAULT_BUFFER_LENGTH)
    process_tree(mixer, buffer=b)
    peak_frequencies = get_peak_frequencies(b.data[0], graph.sample_rate)
    peak_frequencies_rounded = np.round(peak_frequencies, -2)
    assert np.array_equal(peak_frequencies_rounded, frequencies)
