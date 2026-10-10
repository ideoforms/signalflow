import signalflow as sf
import numpy as np
from . import graph

def test_nodes_compressor_below_threshold(graph):
    compressor = sf.Compressor(0.25, threshold=0.5, ratio=4)
    out = graph.render_subgraph_to_new_buffer(compressor, int(graph.sample_rate * 0.1))
    assert np.allclose(out.data[0], 0.25)

def test_nodes_compressor_ratio(graph):
    # Once the envelope has settled, the level above the threshold (in dB) is divided by
    # the ratio: out = threshold * (level / threshold) ^ (1 / ratio).
    for ratio in [1, 2, 20]:
        compressor = sf.Compressor(1.0, threshold=0.5, ratio=ratio, attack_time=0.001)
        out = graph.render_subgraph_to_new_buffer(compressor, int(graph.sample_rate * 0.1))
        assert np.isclose(out.data[0][-1], 0.5 * 2 ** (1 / ratio), rtol=1e-4)

def test_nodes_compressor_brief_peak(graph):
    # A peak lasting a few samples barely moves an envelope with a 2 ms attack, so the
    # signal is not ducked.
    data = np.full(graph.output_buffer_size * 4, 0.3, dtype=np.float32)
    data[4096:4108] = 0.54
    buffer = sf.Buffer(data[np.newaxis])
    compressor = sf.Compressor(sf.BufferPlayer(buffer), threshold=0.5, ratio=20, attack_time=0.002, release_time=0.1)
    out = graph.render_subgraph_to_new_buffer(compressor, len(data))
    assert np.min(out.data[0][4108:4108 + 1000]) > 0.3 * 10 ** (-1 / 20)

def test_nodes_compressor_sidechain(graph):
    # The gain follows the sidechain's level, not the input's.
    compressor = sf.Compressor(0.25, threshold=0.5, ratio=2, attack_time=0.001, sidechain=1.0)
    out = graph.render_subgraph_to_new_buffer(compressor, int(graph.sample_rate * 0.1))
    assert np.isclose(out.data[0][-1], 0.25 * (0.5 / 1.0) ** 0.5, rtol=1e-4)
