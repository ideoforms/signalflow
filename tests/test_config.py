from signalflow import AudioGraph, AudioGraphConfig

def test_config():
    graph = AudioGraph()
    assert graph.config.sample_rate >= 0
