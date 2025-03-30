from signalflow import Buffer, BufferPlayer, BufferRecorder, SineOscillator, AudioGraph, AudioOut_Dummy
from signalflow import SIGNALFLOW_NODE_STATE_ACTIVE, SIGNALFLOW_NODE_STATE_STOPPED
from . import graph

import numpy as np
import pytest

def test_buffer_player(graph):
    data = np.random.uniform(-1, 1, graph.output_buffer_size)
    buf = Buffer(data)
    output = Buffer(1, len(buf) + 1)
    player = BufferPlayer(buf, loop=False)
    graph.play(player)
    assert player.num_input_channels == 1
    assert player.num_output_channels == 1
    assert player.state == SIGNALFLOW_NODE_STATE_ACTIVE
    graph.render_to_buffer(output)
    assert player.state == SIGNALFLOW_NODE_STATE_STOPPED
    assert np.array_equal(output.data[0][:len(buf)], buf.data[0])

def test_buffer_recorder(graph):
    record_buf = Buffer(2, graph.output_buffer_size - 1)

    sine = SineOscillator(440)
    recorder = BufferRecorder(record_buf, sine, loop=False)
    assert recorder.num_input_channels == 2
    assert recorder.num_output_channels == 0
    assert recorder.state == SIGNALFLOW_NODE_STATE_ACTIVE
    graph.add_node(recorder)
    graph.render(len(record_buf) + 1)
    assert recorder.state == SIGNALFLOW_NODE_STATE_STOPPED
    sine_rendered = np.sin(np.arange(len(record_buf)) * np.pi * 2 * 440 / graph.sample_rate)
    assert record_buf.data[0] == pytest.approx(sine_rendered, abs=0.0001)
    assert record_buf.data[1] == pytest.approx(sine_rendered, abs=0.0001)

def test_buffer_recorder_overdub(graph):
    record_buf = Buffer(2, graph.output_buffer_size)

    sine = SineOscillator(440)
    recorder = BufferRecorder(record_buf, sine, feedback=0.5, loop=True)
    assert recorder.num_input_channels == 2
    assert recorder.num_output_channels == 0
    assert recorder.state == SIGNALFLOW_NODE_STATE_ACTIVE

    graph.render_subgraph(recorder, reset=True)
    assert recorder.state == SIGNALFLOW_NODE_STATE_ACTIVE
    sine_rendered = np.sin(np.arange(len(record_buf)) * np.pi * 2 * 440 / graph.sample_rate)
    assert record_buf.data[0] == pytest.approx(sine_rendered, abs=0.001)

    sine2 = SineOscillator(2000)
    recorder.set_input("input", sine2)
    graph.render_subgraph(recorder, reset=True)
    assert recorder.state == SIGNALFLOW_NODE_STATE_ACTIVE
    sine_rendered = 0.5 * sine_rendered + np.sin(np.arange(len(record_buf)) * np.pi * 2 * 2000 / graph.sample_rate)
    assert record_buf.data[0] == pytest.approx(sine_rendered, abs=0.001)

def test_buffer_multichannel_play():
    """
    Test that large multichannel buffers can be rendered successfully
    """
    output_device = AudioOut_Dummy(num_channels=64)
    graph = AudioGraph(output_device=output_device)
    buffer_data = np.ones((64, 2048))
    buffer = Buffer(buffer_data)
    player = BufferPlayer(buffer)
    player.play()
    graph.render()
    assert np.all(graph.output.output_buffer[0] == 1)
    assert np.all(graph.output.output_buffer[63] == 1)
    graph.destroy()