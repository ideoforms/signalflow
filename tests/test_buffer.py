from signalflow import Buffer, Buffer2D, SampleRingBuffer, SampleRingQueue
from signalflow import SIGNALFLOW_INTERPOLATION_MODE_NONE, SIGNALFLOW_INTERPOLATION_MODE_LINEAR
from signalflow import GraphNotCreatedException
import numpy as np
import pytest
import os
from . import graph


def test_buffer_no_graph():
    try:
        b = Buffer(1, 32)
    except GraphNotCreatedException:
        pass


def test_buffer_null(graph):
    b = Buffer()
    assert b.num_channels == 0
    assert b.num_frames == 0
    assert len(b) == 0


def test_buffer(graph):
    b = Buffer(1, 32)
    assert b.num_channels == 1
    assert b.num_frames == 32
    assert len(b) == 32

    data = np.array([[-1, 0, 1], [-1, 0, 1]])
    b = Buffer(2, len(data[0]), data)
    assert b.num_channels == 2
    assert b.num_frames == len(data[0])
    assert np.array_equal(data, b.data)


def test_buffer_instantiate_2d_array(graph):
    data = [[-1, 0, 1], [-1, 0, 1]]
    b = Buffer(data)
    assert b.num_channels == 2
    assert b.num_frames == len(data[0])
    assert np.array_equal(data, b.data)

    data = np.array([[-1, 0, 1], [-1, 0, 1]])
    b = Buffer(data)
    assert b.num_channels == 2
    assert b.num_frames == len(data[0])
    assert np.array_equal(data, b.data)


def test_buffer_instantiate_1d_array(graph):
    data_1d = [-1, 0, 1, 2]
    b = Buffer(data_1d)
    assert b.num_channels == 1
    assert b.num_frames == len(data_1d)
    assert np.array_equal(data_1d, b.data[0])

    data_1d = np.array([-1, 0, 1, 2])
    b = Buffer(data_1d)
    assert b.num_channels == 1
    assert b.num_frames == len(data_1d)
    assert np.array_equal(data_1d, b.data[0])


def test_buffer_multiply(graph):
    b = Buffer([1, 1.5, 2, 2.5])
    b = b * 2
    assert np.array_equal(b.data[0], [2, 3, 4, 5])


def test_buffer_divide(graph):
    b = Buffer([1, 1.5, 2, 2.5])
    b = b - 1
    assert np.array_equal(b.data[0], [0, 0.5, 1, 1.5])


def test_buffer_add(graph):
    b = Buffer([1, 1.5, 2, 2.5])
    b = b + 1
    assert np.array_equal(b.data[0], [2, 2.5, 3, 3.5])


def test_buffer_sub(graph):
    b = Buffer([1, 1.5, 2, 2.5])
    b = b - 1
    assert np.array_equal(b.data[0], [0, 0.5, 1, 1.5])


def test_buffer_subscript(graph):
    data = np.array([[-1, 0, 1], [2, 3, 4]])
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


def test_buffer_write_from_python(graph):
    b = Buffer(2, 32)
    assert b.data[0][0] == 0.0
    b.data[0][1] = 1.0
    assert b.data[0][1] == 1.0
    b.data[1][31] = 1.0
    assert b.data[1][31] == 1.0
    with pytest.raises(IndexError):
        b.data[1][32] = 1.0
    with pytest.raises(IndexError):
        b.data[2][31] = 1.0


def test_buffer_interpolate(graph):
    b = Buffer(1, 2, np.array([[1, 2]]))
    assert b.get(0, 0) == 1
    assert b.get(0, 1) == 2
    assert b.get(0, 0.5) == 1.5
    assert b.get(0, 0.25) == 1.25
    assert b.get(0, -0.5) == 1
    assert b.get(0, 1.5) == 2

    b.interpolation_mode = SIGNALFLOW_INTERPOLATION_MODE_NONE
    assert b.get(0, 0) == 1
    assert b.get(0, 1) == 2
    assert b.get(0, 0.5) == 1
    assert b.get(0, 0.25) == 1


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
    rand_buf = np.array([np.random.uniform(size=buf_len)])
    b = Buffer(1, 44100, rand_buf)
    BUFFER_FILENAME = ".tmp.wav"
    b.save(BUFFER_FILENAME)
    b2 = Buffer(BUFFER_FILENAME)
    assert b2.num_channels == 1
    assert b2.num_frames == buf_len
    assert np.all(b2.data[0] - rand_buf < 0.0001)
    os.unlink(BUFFER_FILENAME)


def test_buffer_2d(graph):
    b1 = Buffer([1, 5, 9])
    b2 = Buffer([2, 4, 5])
    b3 = Buffer([5, 5, 5])

    b2d = Buffer2D([b1, b2, b3])

    assert b2d.get2D(0, 0.00) == 1
    assert b2d.get2D(0, 0.25) == 1.5
    assert b2d.get2D(0, 0.50) == 2
    assert b2d.get2D(0, 0.75) == 3.5
    assert b2d.get2D(0, 1.00) == 5

    assert b2d.get2D(2, 0.00) == 9
    assert b2d.get2D(2, 0.25) == 7
    assert b2d.get2D(2, 0.50) == 5
    assert b2d.get2D(2, 0.75) == 5
    assert b2d.get2D(2, 1.00) == 5

    assert b2d.get2D(1.5, 0.00) == 7
    assert b2d.get2D(1.5, 0.25) == 5.75
    assert b2d.get2D(1.5, 0.50) == 4.5
    assert b2d.get2D(1.5, 0.75) == 4.75
    assert b2d.get2D(1.5, 1.00) == 5

    # TODO: Test with no interpolation


def test_ring_buffer():
    buf = SampleRingBuffer(128)
    assert buf.get_capacity() == 128

    assert buf.get(0) == 0.0
    buf.append(7)
    buf.append(9)
    buf.append(8)
    assert buf.get(0) == 8
    assert buf.get(-1) == 9
    assert buf.get(-2) == 7

    buf.extend([1, 2, 3])
    assert buf.get(0) == 3
    assert buf.get(-1) == 2
    assert buf.get(-2) == 1

def test_ring_queue():
    queue = SampleRingQueue(128)
    assert queue.get_capacity() == 128
    assert queue.get_filled_count() == 0
    queue.append(7)
    queue.append(8)
    assert queue.get_filled_count() == 2
    assert queue.pop() == 7
    assert queue.pop() == 8
    assert queue.get_filled_count() == 0

    queue.extend([1, 2, 3])
    assert queue.pop() == 1
    assert queue.pop() == 2
    assert queue.pop() == 3
