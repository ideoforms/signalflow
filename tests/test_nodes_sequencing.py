from signalflow import *
from . import graph
from . import process_tree

import numpy as np

Fs = 120

def test_nodes_sequencing_sequence(graph):
    graph.sample_rate = Fs
    ar = [1, 2, 3]
    a = Sequence(ar, OneTapDelay(Impulse(4), [1/4, 2/4]))
    graph.render_subgraph(a)
    expected_l = np.repeat([0] + ar, Fs / 4)
    expected_r = np.repeat([0, 0] + ar, Fs / 4)[:Fs]
    assert np.array_equal(a.output_buffer[0][:Fs], expected_l)
    assert np.array_equal(a.output_buffer[1][:Fs], expected_r)

def test_nodes_sequencing_clock_divider(graph):
    graph.sample_rate = Fs
    for divisor in [ 1, 2, 7 ]:
        clock = Impulse(10)
        divider = ClockDivider(clock, divisor)
        graph.render_subgraph(divider)
        indices_orig = np.where(clock.output_buffer[0] == 1)[0]
        indices_div = np.where(divider.output_buffer[0] == 1)[0]
        assert np.array_equal(indices_div, indices_orig[::divisor])

def test_nodes_sequencing_counter(graph):
    graph.sample_rate = Fs
    clock = OneTapDelay(Impulse(8), 1/8)
    counter = Counter(clock, 4, 8)
    graph.render_subgraph(counter)
    expected = np.repeat([ 0, 4, 5, 6, 7, 4, 5, 6 ], Fs/8)
    actual = counter.output_buffer[0][:Fs].astype(int)
    assert np.array_equal(expected, actual)

def test_nodes_sequencing_flipflop(graph):
    graph.sample_rate = Fs
    clock = OneTapDelay(Impulse(4), 1/4)
    flipflop = FlipFlop(clock)
    graph.render_subgraph(flipflop)
    expected = np.repeat([ 0, 1, 0, 1 ], Fs/4)
    actual = flipflop.output_buffer[0][:Fs].astype(int)
    assert np.array_equal(expected, actual)
