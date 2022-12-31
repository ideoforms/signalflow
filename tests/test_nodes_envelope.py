from signalflow import Buffer, Envelope, EnvelopeRect, ADSREnvelope, Impulse, OneTapDelay
from . import graph

import pytest

SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB = -60
SIGNALFLOW_LINEAR_ENVELOPE_MIN = 0.0

def test_envelope_rect(graph):
    # Without a clock signal, generate a single pulse at t = 0
    graph.sample_rate = 1000
    env = EnvelopeRect(0.1)
    graph.render_subgraph(env, reset=True)
    assert env.output_buffer[0][0] == pytest.approx(1.0)
    assert env.output_buffer[0][99] == pytest.approx(1.0)
    assert env.output_buffer[0][100] == pytest.approx(0.0)

    # Trigger 2x envelope pulses at t = 0.1, 0.2
    env = EnvelopeRect(0.01, clock=OneTapDelay(Impulse(10.0), 0.1))
    graph.render_subgraph(env, reset=True)
    assert env.output_buffer[0][0] == pytest.approx(0.0)
    assert env.output_buffer[0][99] == pytest.approx(0.0)
    assert env.output_buffer[0][100] == pytest.approx(1.0)
    assert env.output_buffer[0][109] == pytest.approx(1.0)
    assert env.output_buffer[0][110] == pytest.approx(0.0)
    assert env.output_buffer[0][199] == pytest.approx(0.0)
    assert env.output_buffer[0][200] == pytest.approx(1.0)
    assert env.output_buffer[0][209] == pytest.approx(1.0)
    assert env.output_buffer[0][210] == pytest.approx(0.0)


def test_envelope_adsr(graph):
    graph.sample_rate = 1000

    #--------------------------------------------------------------------------------
    # Sustain for full duration of envelope
    #--------------------------------------------------------------------------------
    gate = Envelope([1, 1, 0], [0.2, 0])
    env = ADSREnvelope(0.01, 0.1, 0.5, 0.05, gate=gate)
    graph.render_subgraph(env, reset=True)
    assert env.output_buffer[0][0] == pytest.approx(0.0)
    assert env.output_buffer[0][10] == pytest.approx(1.0)
    """
    TODO: add test for exponential curve
    assert env.output_buffer[0][110] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB * 0.5))
    assert env.output_buffer[0][199] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB * 0.5))
    assert env.output_buffer[0][250] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB), abs=0.0000001)
    """
    assert env.output_buffer[0][110] == 0.5
    assert env.output_buffer[0][199] == 0.5
    assert env.output_buffer[0][250] == 0.0

    #--------------------------------------------------------------------------------
    # Release early, right after attack segment
    #--------------------------------------------------------------------------------
    gate = Envelope([1, 1, 0], [0.011, 0])
    env = ADSREnvelope(0.01, 0.1, 0.5, 0.05, gate=gate)
    graph.render_subgraph(env, reset=True)
    assert env.output_buffer[0][0] == pytest.approx(0.0)
    assert env.output_buffer[0][10] == pytest.approx(1.0)
    """
    assert env.output_buffer[0][60] == pytest.approx(db_to_amplitude(SIGNALFLOW_EXPONENTIAL_ENVELOPE_MIN_DB), abs=0.0000001)
    """
    assert env.output_buffer[0][61] == 0.0
