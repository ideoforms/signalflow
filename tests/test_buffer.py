from libsignal import Buffer
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