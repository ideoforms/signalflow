from signalflow import AudioGraph, SignalFlowConfig

def test_config():
    graph = AudioGraph()
    assert graph.config.sample_rate >= 0
