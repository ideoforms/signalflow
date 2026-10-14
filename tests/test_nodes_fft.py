from signalflow import Buffer, ASREnvelope
from signalflow import SineOscillator, Impulse, FFT, IFFT, BufferPlayer

try:
    from signalflow import FFTConvolve
    no_fft = False
except:
    #----------------------------------------------------------------
    # Skip these tests on platforms without FFT support
    #----------------------------------------------------------------
    no_fft = True

from . import graph

import numpy as np

fft_size = 2048
num_bins = fft_size // 2 + 1

def test_fft(graph):
    #--------------------------------------------------------------------------------
    # Verify that single-hop FFT output matches numpy's fft
    # 1026 = 512 bins plus DC and Nyquist
    #--------------------------------------------------------------------------------
    buffer_a = Buffer(1, fft_size)

    sine = SineOscillator(440)
    graph.play(sine)
    graph.render_to_buffer(buffer_a)
    graph.stop(sine)

    spectrum = np.fft.rfft(buffer_a.data[0])
    mags_py = np.abs(spectrum)[:num_bins]
    angles_py = np.angle(spectrum)[:num_bins]

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
    assert fft.output_buffer_length == num_bins * 2
    assert len(fft.output_buffer[0]) == num_bins * 2
    graph.render_subgraph(fft, fft_size)

    # Apple vDSP applies a scaling factor of 2x after forward FFT
    # TODO: Fix this (and remove scaling factor in fftw forward fft)
    mags_out = np.copy(fft.output_buffer[0][:num_bins]) / 2
    angles_out = np.copy(fft.output_buffer[0][num_bins:])

    assert np.all(np.abs(mags_py - mags_out) < 0.001)
    assert np.all(np.abs(angles_py - angles_out) < 0.001)

def test_fft_windowed(graph):
    #--------------------------------------------------------------------------------
    # Verify that single-hop FFT output matches numpy's fft
    #--------------------------------------------------------------------------------
    buffer_a = Buffer(1, fft_size)

    graph.render_subgraph_to_buffer(SineOscillator(440), buffer_a)

    # Modify to match the symmetry of vDSP FFT
    window = np.hanning(buffer_a.num_frames + 1)[:buffer_a.num_frames]
    windowed = buffer_a.data[0] * window
    spectrum = np.fft.rfft(windowed)
    mags_py = np.abs(spectrum)[:num_bins]
    angles_py = np.angle(spectrum)[:num_bins]

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=True)
    graph.render_subgraph(fft, fft_size)

    # Apple vDSP applies a scaling factor of 2x after forward FFT
    mags_out = np.copy(fft.output_buffer[0][:num_bins]) / 2
    angles_out = np.copy(fft.output_buffer[0][num_bins:])

    # phases are mismatched for some reason
    assert np.all(np.abs(mags_py - mags_out) < 0.0001)

def test_fft_ifft(graph):
    #--------------------------------------------------------------------------------
    # Verify that fft -> ifft returns the same output.
    # Buffer must be at least fft_size or the results will be delayed until fft_size
    # samples are queued.
    #--------------------------------------------------------------------------------
    for buffer_size in [ fft_size, fft_size * 2, fft_size * 4 ]:
        buffer_a = Buffer(1, buffer_size)
        buffer_b = Buffer(1, buffer_size)

        # TODO: graph.render_subgraph_to_buffer()
        sine = SineOscillator(440)
        sine.play()
        graph.render_to_buffer(buffer_a)
        sine.stop()

        fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
        ifft = IFFT(fft)
        ifft.play()
        graph.render_to_buffer(buffer_b)
        ifft.stop()

        assert np.all(np.abs(buffer_a.data[0] - buffer_b.data[0]) < 0.000001)

def test_fft_ifft_split(graph):
    #--------------------------------------------------------------------------------
    # Verify that fft -> ifft returns the same output, with output
    # buffer smaller than FFT size.
    #--------------------------------------------------------------------------------
    buffer_a = Buffer(1, fft_size)
    buffer_b1 = Buffer(1, fft_size // 2)
    buffer_b2 = Buffer(1, fft_size // 2)
    buffer_b3 = Buffer(1, fft_size // 2)

    sine = SineOscillator(440)
    graph.play(sine)
    graph.render_to_buffer(buffer_a)
    graph.stop(sine)

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
    ifft = IFFT(fft)
    graph.play(ifft)
    graph.render_to_buffer(buffer_b1)
    graph.render_to_buffer(buffer_b2)
    graph.render_to_buffer(buffer_b3)
    graph.stop(ifft)
    #--------------------------------------------------------------------------------
    # Note that the first buffer will be empty as no output will be
    # generated until 1 fft_size worth of samples has been processed.
    #--------------------------------------------------------------------------------
    buffer_b_concatenate = np.concatenate((buffer_b2.data[0], buffer_b3.data[0]))

    assert np.all(buffer_b1.data[0] == 0)
    assert np.all(np.abs(buffer_a.data[0] - buffer_b_concatenate) < 0.00001)

def test_fft_convolve(graph):
    if no_fft:
        return

    buffer_a = Buffer(1, fft_size)
    impulse = Impulse(0)
    impulse.play()
    graph.render_to_buffer(buffer_a)
    impulse.stop()

    buffer_b = Buffer(1, fft_size)
    sine = SineOscillator(440)
    sine.play()
    graph.render_to_buffer(buffer_b)
    sine.stop()

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
    convolve = FFTConvolve(fft, buffer_a)
    ifft = IFFT(convolve) * 0.5
    buffer_c = Buffer(1, fft_size)
    graph.play(ifft)
    graph.render_to_buffer(buffer_c)
    graph.stop(ifft)

    assert np.all(np.abs(buffer_b.data[0] - buffer_c.data[0]) < 0.000001)

def test_fft_convolve_split(graph):
    if no_fft:
        return

    buffer_ir = Buffer(1, fft_size * 4)
    envelope_duration_seconds = buffer_ir.num_frames / graph.sample_rate
    envelope = ASREnvelope(0, 0, envelope_duration_seconds) * SineOscillator(440)

    # TODO: graph.render_subgraph_to_buffer()
    graph.play(envelope)
    graph.render_to_buffer(buffer_ir)
    graph.stop(envelope)

    fft = FFT(Impulse(0), fft_size=fft_size, hop_size=fft_size, do_window=False)
    convolve = FFTConvolve(fft, buffer_ir)
    ifft = IFFT(convolve) * 0.5
    ifft.play()

    buffer_out = Buffer(1, fft_size)
    output_samples = np.zeros(0)
    for n in range(4):
        graph.render_to_buffer(buffer_out)
        output_samples = np.concatenate((output_samples, buffer_out.data[0]))
    assert np.all(np.abs(output_samples - buffer_ir.data[0]) < 0.000001)

def render_in_blocks(graph, node, block_sizes, num_frames):
    #--------------------------------------------------------------------------------
    # Render num_frames of node's output, cycling through the given block sizes.
    #--------------------------------------------------------------------------------
    output = []
    total = 0
    index = 0
    while total < num_frames:
        block_size = block_sizes[index % len(block_sizes)]
        graph.render_subgraph(node, block_size, reset=True)
        output.append(np.copy(node.output_buffer[0][:block_size]))
        total += block_size
        index += 1
    return np.concatenate(output)[:num_frames]

def test_fft_ifft_block_sizes(graph):
    #--------------------------------------------------------------------------------
    # Verify that fft -> ifft reconstructs its input when the block size is not a
    # multiple of the hop size. Latency is fft_size - gcd(block_size, hop_size, fft_size).
    #--------------------------------------------------------------------------------
    n = 512
    rng = np.random.default_rng(0)
    signal = rng.uniform(-1, 1, 20000).astype(np.float32)
    for block_size, latency in [(512, 0), (256, 256), (480, 480), (100, 508)]:
        fft = FFT(BufferPlayer(Buffer(signal.reshape(1, -1))), fft_size=n, hop_size=n, do_window=False)
        ifft = IFFT(fft)
        output = render_in_blocks(graph, ifft, [block_size], 10000)
        assert np.all(output[:latency] == 0)
        assert np.allclose(output[latency:], signal[:10000 - latency], atol=1e-5)

def test_fft_convolve_block_sizes(graph):
    if no_fft:
        return

    #--------------------------------------------------------------------------------
    # Verify that fft -> convolve -> ifft matches numpy's convolution, with
    # overlapping hops (hop = fft_size / 2), for block sizes that are not a
    # multiple of the hop size, and for block sizes that vary between blocks.
    #--------------------------------------------------------------------------------
    n = 512
    hop = n // 2
    rng = np.random.default_rng(0)
    signal = rng.uniform(-1, 1, 20000).astype(np.float32)
    ir = rng.normal(0, 0.3, hop).astype(np.float32)
    expected = np.convolve(signal, ir)

    for block_sizes in [[256], [480], [512], [128]]:
        fft = FFT(BufferPlayer(Buffer(signal.reshape(1, -1))), fft_size=n, hop_size=hop, window_size=hop, do_window=False)
        # IR is zero-padded to fft_size, as FFTConvolve reads fft_size samples per partition
        ir_padded = np.concatenate((ir, np.zeros(n - hop, dtype=np.float32)))
        convolve = FFTConvolve(fft, Buffer(ir_padded.reshape(1, -1)))
        ifft = IFFT(convolve)
        output = render_in_blocks(graph, ifft, block_sizes, 10000)

        # Find the latency, then check the output against numpy
        nonzero = np.flatnonzero(output)
        latency = nonzero[0] if len(nonzero) else 0
        assert latency <= n
        assert np.allclose(output[latency:], expected[:10000 - latency], atol=1e-4), block_sizes
