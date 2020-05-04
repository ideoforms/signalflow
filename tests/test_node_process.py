from libsignal import Sine, AudioGraph

import pytest

def test_process():
    a = Sine(440)
    with pytest.raises(Exception):
        a.process(1024)

    graph = AudioGraph()
    a = Sine(440)
    a.process(1024)
    assert a.output_buffer.shape == (32, 1024)
    del graph