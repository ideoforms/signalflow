import signalflow as sf
from . import graph

import numpy as np
import pytest


def test_nodes_oscillators_sine_lfo(graph):
    a = sf.SineLFO([1, 2.5], phase=[0, 0.5])
    graph.render_subgraph(a)

    expected = np.sin(np.arange(a.output_buffer_length) * np.pi * 2 * 1.0 / graph.sample_rate) * 0.5 + 0.5
    assert a.output_buffer[0] == pytest.approx(expected, abs=0.0001)

    expected = np.sin(
        (np.arange(a.output_buffer_length) * np.pi * 2 * 2.5) / graph.sample_rate + (np.pi * 2 * 0.5)) * 0.5 + 0.5
    assert a.output_buffer[1] == pytest.approx(expected, abs=0.0001)


def test_nodes_oscillators_saw_lfo(graph):
    a = sf.SawLFO([1, 2], phase=[0, 0.5])
    graph.sample_rate = 16
    graph.render_subgraph(a, graph.sample_rate)

    expected0 = np.arange(0, 1, 1 / graph.sample_rate)
    assert a.output_buffer[0] == pytest.approx(expected0)

    expected1 = np.tile(np.arange(0, 1, 2 / graph.sample_rate), 2)
    expected1_shifted = np.roll(expected1, -graph.sample_rate // (2 * 2))
    assert a.output_buffer[1] == pytest.approx(expected1_shifted)


def test_nodes_oscillators_triangle_lfo(graph):
    a = sf.TriangleLFO([1, 2], phase=[0, 0.5])
    graph.sample_rate = 16
    graph.render_subgraph(a, graph.sample_rate)

    expected0 = np.concatenate((np.arange(0, 1, 2 / graph.sample_rate), np.arange(1, 0, -2 / graph.sample_rate)))
    assert a.output_buffer[0] == pytest.approx(expected0, rel=0.00001)

    expected1 = np.concatenate((
        np.arange(1, 0, -4 / graph.sample_rate),
        np.arange(0, 1, 4 / graph.sample_rate),
        np.arange(1, 0, -4 / graph.sample_rate),
        np.arange(0, 1, 4 / graph.sample_rate),
    ))
    assert a.output_buffer[1] == pytest.approx(expected1, rel=0.00001)


def test_nodes_oscillators_square_lfo(graph):
    a = sf.SquareLFO([1, 2], phase=[0, 0.5])
    graph.sample_rate = 16
    graph.render_subgraph(a, graph.sample_rate)

    expected0 = np.concatenate((
        np.full(graph.sample_rate // 2, 1),
        np.full(graph.sample_rate // 2, 0)
    ))
    assert a.output_buffer[0] == pytest.approx(expected0)

    expected0 = np.concatenate((
        np.full(graph.sample_rate // 4, 0),
        np.full(graph.sample_rate // 4, 1),
        np.full(graph.sample_rate // 4, 0),
        np.full(graph.sample_rate // 4, 1),
    ))
    assert a.output_buffer[1] == pytest.approx(expected0)
