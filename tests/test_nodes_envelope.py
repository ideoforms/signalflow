from signalflow import Buffer, EnvelopeADSR
from signalflow import db_to_amplitude
from . import graph
from . import process_tree

import numpy as np
import pytest

SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB = -60
SIGNALFLOW_LINEAR_ENVELOPE_MIN = 0.0

def test_envelope_adsr(graph):
    graph.sample_rate = 1000

    #--------------------------------------------------------------------------------
    # feedback = 0
    # Input signal is passed through unmodified
    #--------------------------------------------------------------------------------
    env = EnvelopeADSR(0.01, 0.1, 0.5, 1.0)
    b = Buffer(1, 2000)
    process_tree(env, buffer=b)
    print(b.data[0][10])
    assert b.data[0][0] == 0
    assert b.data[0][10] == pytest.approx(1.0)
    assert b.data[0][110] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB * 0.5))
    assert b.data[0][1111] == 0.0
