from signalflow import RandomImpulse, Buffer
from signalflow import SIGNALFLOW_EVENT_DISTRIBUTION_POISSON
import signalflow as sf
from . import graph, process_tree

import numpy as np


def test_random_impulse(graph):
    frequency = 1000

    b = Buffer(1, graph.sample_rate)
    a = RandomImpulse(frequency)
    process_tree(a, buffer=b)
    impulse_count = np.sum(b.data[0])
    assert (impulse_count - frequency) < (0.1 * frequency)

    a = RandomImpulse(frequency, SIGNALFLOW_EVENT_DISTRIBUTION_POISSON)
    process_tree(a, buffer=b)
    impulse_count = np.sum(b.data[0])
    assert (impulse_count - frequency) < (0.1 * frequency)


def test_random_uniform(graph):
    clock = sf.Node()
    a = sf.RandomUniform(min=[10, 30], max=[20, 40], clock=clock)

    # Random initial values
    graph.render_subgraph(a)
    value00 = a.output_buffer[0][0]
    assert value00 >= 10 and value00 <= 20
    assert all(a.output_buffer[0][1:] == value00)
    value01 = a.output_buffer[1][0]
    assert value01 >= 30 and value01 <= 40
    assert all(a.output_buffer[1][1:] == value01)

    # New random values after trigger() called
    a.trigger()
    graph.render_subgraph(a, reset=True)
    value10 = a.output_buffer[0][0]
    assert value10 >= 10 and value10 <= 20
    assert value00 != value10
    assert all(a.output_buffer[0] == value10)
    value11 = a.output_buffer[1][0]
    assert value11 >= 30 and value11 <= 40
    assert all(a.output_buffer[1] == value11)

    # New random values after audio-rate trigger
    clock.output_buffer[0][9] = 1.0
    graph.render_subgraph(a, reset=True)
    assert all(a.output_buffer[0][:9] == value10)
    assert all(a.output_buffer[0][9:] != value10)


def _test_stochastic_node(graph, node, value_condition):
    node.set_seed(123)
    graph.render_subgraph(node)
    values = node.output_buffer[0].copy()
    assert value_condition(values)
    graph.render_subgraph(node, reset=True)
    assert np.any(values != node.output_buffer[0])
    node.set_seed(123)
    graph.render_subgraph(node, reset=True)
    assert np.all(values == node.output_buffer[0])


def test_random_uniform_seed(graph):
    _test_stochastic_node(graph, sf.RandomUniform(0, 1),
                          lambda values: np.all(values > 0) and np.all(values < 1))


def test_random_gaussian_seed(graph):
    _test_stochastic_node(graph, sf.RandomGaussian(3, 0.1),
                          lambda values: np.all(values > 2) and np.all(values < 4))


def test_random_exponential_seed(graph):
    _test_stochastic_node(graph, sf.RandomExponential(1, 10),
                          lambda values: np.all(values > 1) and np.all(values < 10))

def test_random_coin_seed(graph):
    _test_stochastic_node(graph, sf.RandomCoin(0.5),
                          lambda values: np.all((values == 0) | (values == 1)))
