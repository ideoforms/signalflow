from signalflow import Buffer, BufferPlayer, BufferRecorder
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