from libsignal import Buffer
from libsignal import SIGNAL_INTERPOLATION_NONE, SIGNAL_INTERPOLATION_LINEAR
import numpy as np

def test_buffer():
    b = Buffer(1, 32)
    assert b.num_channels == 1
    assert b.num_frames == 32

    data = np.array([[ -1, 0, 1 ], [ -1, 0, 1 ]])
    b = Buffer(2, len(data[0]), data)
    assert b.num_channels == 2
    assert b.num_frames == len(data[0])
    assert np.array_equal(data, b.data)

def test_buffer_interpolation():
    b = Buffer(1, 2, np.array([[ 1, 2 ]]))
    assert b.get(0) == 1
    assert b.get(1) == 2
    assert b.get(0.5) == 1.5
    assert b.get(0.25) == 1.25
    assert b.get(-0.5) == 1
    assert b.get(1.5) == 2

    b.interpolate = SIGNAL_INTERPOLATION_NONE
    assert b.get(0) == 1
    assert b.get(1) == 2
    assert b.get(0.5) == 1
    assert b.get(0.25) == 1