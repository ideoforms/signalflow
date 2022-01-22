from signalflow import AudioGraph, AudioGraphConfig

@pytest.mark.skip
def test_config():
    graph = AudioGraph()
    assert graph.config.sample_rate >= 0
