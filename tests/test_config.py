from signalflow import AudioGraph, AudioGraphConfig
import pytest

@pytest.mark.skip
def test_config():
    graph = AudioGraph()
    assert graph.config.sample_rate >= 0
