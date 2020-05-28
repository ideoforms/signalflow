from libsignal import Buffer
from libsignal import SIGNAL_INTERPOLATION_NONE, SIGNAL_INTERPOLATION_LINEAR
import numpy as np
import pytest
import os
from . import graph

def test_buffer(graph):
    b = Buffer(1, 32)
    assert b.num_channels == 1
    assert b.num_frames == 32
    assert len(b) == 32

    data = np.array([[ -1, 0, 1 ], [ -1, 0, 1 ]])
    b = Buffer(2, len(data[0]), data)
    assert b.num_channels == 2
    assert b.num_frames == len(data[0])
    assert np.array_equal(data, b.data)

    b = Buffer(data)
    assert b.num_channels == 2
    assert b.num_frames == len(data[0])
    assert np.array_equal(data, b.data)

    data_1d = np.array([ -1, 0, 1, 2 ])
    b = Buffer(data_1d)
    assert b.num_channels == 1
    assert b.num_frames == len(data_1d)
    assert np.array_equal(data_1d, b.data[0])

def test_buffer_multiply(graph):
    b = Buffer([ 1, 1.5, 2, 2.5 ])
    b = b * 2
    assert np.array_equal(b.data[0], [ 2, 3, 4, 5 ])

def test_buffer_subscript(graph):
    data = np.array([[ -1, 0, 1 ], [ 2, 3, 4 ]])
    b = Buffer(data)
    b0 = b[0]
    b1 = b[1]
    assert b0.num_channels == 1
    assert np.array_equal(b0.data[0], data[0])
    assert b1.num_channels == 1
    assert np.array_equal(b1.data[0], data[1])

    with pytest.raises(Exception):
        _ = b[2]
    with pytest.raises(Exception):
        _ = b[-1]

def test_buffer_interpolate(graph):
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

def test_buffer_fill(graph):
    b = Buffer(4, 44100)
    b.fill(0.5)
    assert b.data.shape == (4, 44100)
    assert np.all(b.data[0] == 0.5)
    assert np.all(b.data[1] == 0.5)
    assert np.all(b.data[2] == 0.5)
    assert np.all(b.data[3] == 0.5)

def test_buffer_split(graph):
    b = Buffer(1, 8192)
    b.fill(1)
    buffers = b.split(2048)
    assert len(buffers) == 4
    for buf in buffers:
        assert buf.num_channels == 1
        assert buf.num_frames == 2048
        assert np.all(b.data[0] == 1)

def test_buffer_load(graph):
    b = Buffer("examples/audio/gliss.aif")
    assert b.data.shape == (1, 262856)
    assert b.num_frames == 262856
    assert b.num_channels == 1
    assert b.sample_rate == 44100
    assert b.duration == pytest.approx(5.960453)
    rms = np.sqrt(np.mean(np.square(b.data[0])))
    assert rms == pytest.approx(0.08339643)

def test_buffer_save(graph):
    buf_len = 44100
    rand_buf = np.array([ np.random.uniform(size=buf_len) ])
    b = Buffer(1, 44100, rand_buf)
    BUFFER_FILENAME = ".tmp.wav"
    b.save(BUFFER_FILENAME)
    b2 = Buffer(BUFFER_FILENAME)
    assert b2.num_channels == 1
    assert b2.num_frames == buf_len
    assert np.all(b2.data[0] - rand_buf < 0.0001)
    os.unlink(BUFFER_FILENAME)
