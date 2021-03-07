from signalflow import Buffer, Envelope, EnvelopeADSR
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
    # Sustain for full duration of envelope
    #--------------------------------------------------------------------------------
    gate = Envelope([1, 1, 0], [0.2, 0])
    env = EnvelopeADSR(0.01, 0.1, 0.5, 0.05, gate=gate)
    graph.render_subgraph(env, reset=True)
    assert env.output_buffer[0][0] == pytest.approx(0.0)
    assert env.output_buffer[0][10] == pytest.approx(1.0)
    assert env.output_buffer[0][110] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB * 0.5))
    assert env.output_buffer[0][199] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB * 0.5))
    assert env.output_buffer[0][250] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB), abs=0.0000001)

    #--------------------------------------------------------------------------------
    # Release early, right after attack segment
    #--------------------------------------------------------------------------------
    gate = Envelope([1, 1, 0], [0.011, 0])
    env = EnvelopeADSR(0.01, 0.1, 0.5, 0.05, gate=gate)
    graph.render_subgraph(env, reset=True)
    assert env.output_buffer[0][0] == pytest.approx(0.0)
    assert env.output_buffer[0][10] == pytest.approx(1.0)
    assert env.output_buffer[0][60] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB), abs=0.0000001)