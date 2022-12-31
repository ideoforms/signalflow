from signalflow import Impulse, CombDelay, OneTapDelay, Buffer
from . import graph

import numpy as np

def test_comb_delay(graph):
    graph.sample_rate = 100

    #--------------------------------------------------------------------------------
    # feedback = 0
    # Input signal is passed through unmodified
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a = CombDelay(i, 0.1, 0.0)
    graph.render_subgraph(a, reset=True)
    output = a.output_buffer[0]
    assert output[0] == 1.0
    assert np.all(output[1:] == 0.0)

    #--------------------------------------------------------------------------------
    # feedback = 0.5
    # Input signal is passed through with 50% feedback
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a = CombDelay(i, 0.1, 0.5)
    graph.render_subgraph(a, reset=True)
    output = a.output_buffer[0]
    assert output[0] == 1.0
    assert np.all(output[1:10] == 0.0)
    assert output[10] == 0.5
    assert np.all(output[11:20] == 0.0)
    assert output[20] == 0.25

def test_one_tap_delay(graph):
    graph.sample_rate = 100

    #--------------------------------------------------------------------------------
    # Test one-tap delay with a delay time that is an integer # of samples
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a = OneTapDelay(i, 0.1)
    graph.render_subgraph(a, reset=True)
    output = a.output_buffer[0]
    assert np.all(output[:10] == 0.0)
    assert output[10] == 1.0
    assert np.all(output[11:] == 0.0)

    #--------------------------------------------------------------------------------
    # Test delay with a fractional # samples and interpolation
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a.set_input("delay_time", 0.095)
    a.set_input("input", i)
    graph.render_subgraph(a, reset=True)

    assert np.all(output[:9] == 0.0)
    assert output[9] == 0.5
    assert output[10] == 0.5
    assert np.all(output[11:] == 0.0)