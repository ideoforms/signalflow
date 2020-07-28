from signalflow import Buffer, BufferPlayer, BufferRecorder, Sine
from signalflow import SIGNAL_NODE_STATE_ACTIVE, SIGNAL_NODE_STATE_FINISHED
from . import graph
from . import process_tree

import numpy as np
import pytest

def test_buffer_player(graph):
    data = np.random.uniform(-1, 1, 1024)
    buf = Buffer(data)
    output = Buffer(1, len(buf) + 1)
    player = BufferPlayer(buf, loop=False)
    assert player.num_input_channels == 1
    assert player.num_output_channels == 1
    assert player.state == SIGNAL_NODE_STATE_ACTIVE
    process_tree(player, buffer=output)
    assert player.state == SIGNAL_NODE_STATE_FINISHED
    assert np.array_equal(output.data[0][:len(buf)], buf.data[0])

def test_buffer_recorder(graph):
    record_buf = Buffer(2, 1024)

    sine = Sine(440)
    recorder = BufferRecorder(record_buf, sine, loop=False)
    assert recorder.num_input_channels == 2
    assert recorder.num_output_channels == 0
    assert recorder.state == SIGNAL_NODE_STATE_ACTIVE
    process_tree(recorder, num_frames=len(record_buf) + 1)
    assert recorder.state == SIGNAL_NODE_STATE_FINISHED
    sine_rendered = np.sin(np.arange(len(record_buf)) * np.pi * 2 * 440 / graph.sample_rate)
    assert list(record_buf.data[0]) == pytest.approx(sine_rendered, abs=0.0001)
    