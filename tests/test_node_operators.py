from signalflow import Constant, ChannelArray, Sum
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
