import signalflow as sf
from . import graph

import numpy as np
import pytest


@pytest.fixture(scope="function")
def granulator(graph):
    graph.sample_rate = 1000

    buf = sf.Buffer(1, 5)
    buf.fill(1.0)
    granulator = sf.Granulator(buf, duration=buf.duration)
    granulator.set_buffer("envelope", sf.EnvelopeBuffer("rectangular"))

    return granulator


def test_nodes_granulator(graph, granulator):
    # Without any triggers, output is zero
    graph.render_subgraph(granulator)
    assert np.all(granulator.output_buffer[0] == 0.0)


def test_nodes_granulator_intervals(graph, granulator):
    # Trigger once every 10 samples.
    clock = sf.Impulse(100)
    granulator.clock = clock
    graph.render_subgraph(granulator)
    assert np.all(granulator.output_buffer[0][:1000] == np.tile([0.5, 0.5, 0.5, 0.5, 0.5,
                                                                 0.0, 0.0, 0.0, 0.0, 0.0], 100))


def test_nodes_granulator_overlap(graph, granulator):
    # Trigger once every 4 samples, check that overlap is applied properly
    clock = sf.Impulse(250)
    granulator.clock = clock
    graph.render_subgraph(granulator)
    assert np.all(granulator.output_buffer[0][:10] == [0.5, 0.5, 0.5, 0.5, 1.0,
                                                       0.5, 0.5, 0.5, 1.0, 0.5])


def test_nodes_granulator_envelope(graph, granulator):
    # Trigger once every 4 samples, check that overlap is applied properly
    clock = sf.Impulse(0)
    envelope = sf.EnvelopeBuffer([0.25, 0.5, 1.0, 0.5, 0.25])
    granulator.clock = clock
    granulator.set_buffer("envelope", envelope)
    graph.render_subgraph(granulator)
    assert np.all(granulator.output_buffer[0][:10] == [0.125, 0.25, 0.5, 0.25, 0.125,
                                                       0, 0, 0, 0, 0])
