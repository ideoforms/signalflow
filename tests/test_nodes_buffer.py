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
    graph.add_node(recorder)
    graph.render(len(record_buf) + 1)
    assert recorder.state == SIGNAL_NODE_STATE_FINISHED
    sine_rendered = np.sin(np.arange(len(record_buf)) * np.pi * 2 * 440 / graph.sample_rate)
    assert list(record_buf.data[0]) == pytest.approx(sine_rendered, abs=0.0001)
    assert list(record_buf.data[1]) == pytest.approx(sine_rendered, abs=0.0001)

    sine2 = Sine(2000)
    recorder2 = BufferRecorder(record_buf, sine2, feedback=0.5, loop=True)
    assert recorder2.num_input_channels == 2
    assert recorder2.num_output_channels == 0
    assert recorder2.state == SIGNAL_NODE_STATE_ACTIVE

    process_tree(recorder2, num_frames=len(record_buf))
    assert recorder2.state == SIGNAL_NODE_STATE_ACTIVE
    sine_rendered2 = 0.5 * sine_rendered + np.sin(np.arange(len(record_buf)) * np.pi * 2 * 2000 / graph.sample_rate)
    assert list(record_buf.data[0]) == pytest.approx(sine_rendered2, abs=0.001)

    sine3 = Sine(2000)
    recorder2.set_input("input", sine3)
    process_tree(recorder2, num_frames=len(record_buf))
    assert recorder2.state == SIGNAL_NODE_STATE_ACTIVE
    sine_rendered2 = 0.5 * sine_rendered2 + np.sin(np.arange(len(record_buf)) * np.pi * 2 * 2000 / graph.sample_rate)
    assert list(record_buf.data[0]) == pytest.approx(sine_rendered2, abs=0.001)