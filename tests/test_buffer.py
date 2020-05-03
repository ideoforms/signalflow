from libsignal import Buffer
import numpy as np

def test_buffer():
    b = Buffer(1, 32)
    assert b.num_channels == 1
    assert b.num_frames == 32
