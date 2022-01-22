from signalflow import AudioGraph, AudioGraphConfig
import pytest

def test_config():
    graph = AudioGraph(start=False)
    assert graph.config.sample_rate >= 0
