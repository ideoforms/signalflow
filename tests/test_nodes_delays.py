from libsignal import Impulse, CombDelay, OneTapDelay, Buffer
from . import graph
from . import process_tree

import numpy as np
import pytest

def test_comb_delay(graph):
    graph.sample_rate = 100

    #--------------------------------------------------------------------------------
    # feedback = 0
    # Input signal is passed through unmodified
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a = CombDelay(i, 0.1, 0.0)
    b = Buffer(1, 100)
    process_tree(a, buffer=b)
    assert b.data[0][0] == 1.0
    assert np.all(b.data[0][1:] == 0.0)

    #--------------------------------------------------------------------------------
    # feedback = 0.5
    # Input signal is passed through with 50% feedback
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a = CombDelay(i, 0.1, 0.5)
    b = Buffer(1, 100)
    process_tree(a, buffer=b)
    assert b.data[0][0] == 1.0
    assert np.all(b.data[0][1:10] == 0.0)
    assert b.data[0][10] == 0.5
    assert np.all(b.data[0][11:20] == 0.0)
    assert b.data[0][20] == 0.25

def test_one_tap_delay(graph):
    graph.sample_rate = 100

    #--------------------------------------------------------------------------------
    # Test one-tap delay with a delay time that is an integer # of samples
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a = OneTapDelay(i, 0.1)
    b = Buffer(1, 100)
    process_tree(a, buffer=b)
    assert np.all(b.data[0][:10] == 0.0)
    assert b.data[0][10] == 1.0
    assert np.all(b.data[0][11:] == 0.0)

    #--------------------------------------------------------------------------------
    # Test delay with a fractional # samples and interpolation
    #--------------------------------------------------------------------------------
    i = Impulse(0)
    a.set_input("delay_time", 0.095)
    a.set_input("input", i)
    process_tree(a, buffer=b)

    assert np.all(b.data[0][:9] == 0.0)
    assert b.data[0][9] == 0.5
    assert b.data[0][10] == 0.5
    assert np.all(b.data[0][11:] == 0.0)