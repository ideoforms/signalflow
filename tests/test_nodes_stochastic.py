from libsignal import RandomImpulse, Buffer
from libsignal import SIGNAL_EVENT_DISTRIBUTION_POISSON
from . import graph, process_tree

import numpy as np

def test_random_impulse(graph):
    frequency = 1000

    b = Buffer(1, graph.sample_rate)
    a = RandomImpulse(frequency)
    process_tree(a, buffer=b)
    impulse_count = np.sum(b.data[0])
    assert (impulse_count - frequency) < (0.1 * frequency)

    a = RandomImpulse(frequency, SIGNAL_EVENT_DISTRIBUTION_POISSON)
    process_tree(a, buffer=b)
    impulse_count = np.sum(b.data[0])
    assert (impulse_count - frequency) < (0.1 * frequency)
