from signalflow import Constant, ChannelArray, ChannelOffset, Sum, SelectInput, Counter, Impulse, Bus
from signalflow import ChannelCrossfade
import numpy as np

from . import graph


def test_add(graph):
    a = Constant(1)
    b = Constant(2)

    c = a + b
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 3)

    c = a + b + 2
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 5)

    c = a + 1
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 2)

    c = a + 1.5
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 2.5)

    c = 3 + a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 4)

    c = 3.5 + a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 4.5)


def test_subtract(graph):
    a = Constant(1)
    b = Constant(2)

    c = a - b
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == -1)

    c = a - b - 2
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == -3)

    c = a - 1
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 0)

    c = a - 1.5
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == -0.5)

    c = 3 - a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 2)

    c = 3.5 - a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 2.5)


def test_multiply(graph):
    a = Constant(2)
    b = Constant(3)

    c = a * b
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 6)

    c = a * b * 2
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 12)

    c = a * 2
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 4)

    c = a * 1.5
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 3)

    c = 3 * a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 6)

    c = 3.5 * a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 7)


def test_divide(graph):
    a = Constant(1)
    b = Constant(2)

    c = a / b
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 0.5)

    c = a / b / 2
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 0.25)

    c = a / 2
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 0.5)

    c = a / 2.5
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 0.4)

    c = 3 / b
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 1.5)

    c = 3.5 / b
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 1.75)


def test_pow(graph):
    a = Constant(2)
    b = Constant(4)

    c = a ** b
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 16)

    c = b ** a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 16)

    c = a ** 2
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 4)

    c = b ** 2.5
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 32)

    c = 2 ** a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 4)

    c = 4 ** a
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 16)


def test_subscript(graph):
    a = ChannelArray([1, 2, 3, 4])
    assert a.num_output_channels == 4
    graph.render_subgraph(a)
    assert np.all(a.output_buffer[0] == 1)
    assert np.all(a.output_buffer[1] == 2)
    assert np.all(a.output_buffer[2] == 3)
    assert np.all(a.output_buffer[3] == 4)

    b = a[0]
    graph.render_subgraph(b)
    assert b.num_output_channels == 1
    assert np.all(b.output_buffer[0] == 1)
    assert np.all(b.output_buffer[1] == 0)
    b = a[1]
    graph.render_subgraph(b)
    assert b.num_output_channels == 1
    assert np.all(b.output_buffer[0] == 2)
    assert np.all(b.output_buffer[1] == 0)
    b = a[2]
    graph.render_subgraph(b)
    assert np.all(b.output_buffer[0] == 3)
    assert np.all(b.output_buffer[1] == 0)
    b = a[3]
    graph.render_subgraph(b)
    assert np.all(b.output_buffer[0] == 4)
    assert np.all(b.output_buffer[1] == 0)

    b = a[0:2]
    graph.render_subgraph(b)
    assert np.all(b.output_buffer[0] == 1)
    assert np.all(b.output_buffer[1] == 2)
    assert np.all(b.output_buffer[2] == 0)

    b = a[0:4:2]
    graph.render_subgraph(b)
    assert np.all(b.output_buffer[0] == 1)
    assert np.all(b.output_buffer[1] == 3)
    assert np.all(b.output_buffer[2] == 0)


def test_sum(graph):
    a = Sum([1, 2, 3, 4])
    assert a.num_output_channels == 1
    graph.render_subgraph(a)
    assert np.all(a.output_buffer[0] == 10)

    a = ChannelArray([1, 1])
    b = ChannelArray([2, 2, 2])
    c = Sum([a, b, 3])
    graph.render_subgraph(c)
    assert np.all(c.output_buffer[0] == 6)
    assert np.all(c.output_buffer[1] == 6)
    assert np.all(c.output_buffer[2] == 6)

def test_channel_offset(graph):
    a = ChannelArray([1, 2])
    b = ChannelOffset(0, a)
    graph.render_subgraph(b, reset=True)
    assert b.num_output_channels == 2
    assert np.all(b.output_buffer[0] == 1)
    assert np.all(b.output_buffer[1] == 2)

    b = ChannelOffset(2, a)
    graph.render_subgraph(b, reset=True)
    assert b.num_output_channels == 4
    assert np.all(b.output_buffer[0] == 0)
    assert np.all(b.output_buffer[1] == 0)
    assert np.all(b.output_buffer[2] == 1)
    assert np.all(b.output_buffer[3] == 2)


def test_select_input(graph):
    a = SelectInput([1, 2, 3], 0)

    graph.render_subgraph(a, reset=True)
    assert np.all(a.output_buffer[0] == 1)

    a.set_input("index", 1)
    graph.render_subgraph(a, reset=True)
    assert np.all(a.output_buffer[0] == 2)

    a.set_input("index", 2)
    graph.render_subgraph(a, reset=True)
    assert np.all(a.output_buffer[0] == 3)

    # test audio-rate modulation and wraparound
    a.set_input("index", Counter(Impulse(graph.sample_rate // 4), 0, 3))
    graph.render_subgraph(a, reset=True)
    assert np.all(a.output_buffer[0][0:4] == 1)
    assert np.all(a.output_buffer[0][4:8] == 2)
    assert np.all(a.output_buffer[0][8:12] == 3)
    assert np.all(a.output_buffer[0][12:16] == 1)

    # test multichannel upmix behaviour -
    # should always honour the max number of input channels
    a = ChannelArray([1, 2])
    b = Constant(3)
    c = SelectInput([a, b], index=0)
    assert a.num_output_channels == 2
    assert b.num_output_channels == 1
    assert c.index.num_output_channels == 1
    assert c.num_output_channels == 2

    graph.render_subgraph(c, reset=True)
    assert np.all(c.output_buffer[0] == 1)
    assert np.all(c.output_buffer[1] == 2)

    c.index = 1
    graph.render_subgraph(c, reset=True)
    assert np.all(c.output_buffer[0] == 3)
    assert np.all(c.output_buffer[1] == 3)

def test_bus(graph):
    a = Bus(2)
    graph.render_subgraph(a, reset=True)
    assert a.num_input_channels == 2
    assert a.num_output_channels == 2
    assert np.all(a.output_buffer[0] == 0)
    assert np.all(a.output_buffer[1] == 0)

    b = Constant(3)
    c = ChannelArray([4, 5])
    a.add_input(b)
    a.add_input(c)
    graph.render_subgraph(a, reset=True)
    assert np.all(a.output_buffer[0] == 7)
    assert np.all(a.output_buffer[1] == 8)

    a.remove_input(b)
    graph.render_subgraph(a, reset=True)
    assert np.all(a.output_buffer[0] == 4)
    assert np.all(a.output_buffer[1] == 5)

    a.remove_input(c)
    graph.render_subgraph(a, reset=True)
    assert np.all(a.output_buffer[0] == 0)
    assert np.all(a.output_buffer[1] == 0)

def test_crossfade(graph):
    a = ChannelArray([1, 2, 3, 4])
    b = ChannelCrossfade(a, Constant(0))

    graph.render_subgraph(b, reset=True)
    assert np.all(b.output_buffer[0] == 1)

    b.set_input("index", 1)
    graph.render_subgraph(b, reset=True)
    assert np.all(b.output_buffer[0] == 2)

    b.set_input("index", 2)
    graph.render_subgraph(b, reset=True)
    assert np.all(b.output_buffer[0] == 3)

    b.set_input("index", 2.5)
    graph.render_subgraph(b, reset=True)
    assert np.all(b.output_buffer[0] == 3.5)

    c = ChannelCrossfade(ChannelArray([0.0, 1.0]), 0.5, type="equal-power")
    graph.render_subgraph(c, reset=True)
    assert np.allclose(c.output_buffer[0], np.sqrt(0.5))