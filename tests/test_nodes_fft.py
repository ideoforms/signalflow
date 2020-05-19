try:
    from libsignal import Sine, Impulse, FFT, IFFT, FFTConvolve, EnvelopeASR
    from libsignal import Buffer
    no_fft = False
except:
    #----------------------------------------------------------------
    # Skip these tests on platforms without FFT support
    #----------------------------------------------------------------
    no_fft = True

from . import graph
from . import process_tree

import numpy as np

def test_fft(graph):
    if no_fft:
        return

    #----------------------------------------------------------------
    # Verify that single-hop FFT output matches numpy's fft
    #----------------------------------------------------------------
    buffer_a = Buffer(1, 1024)
    buffer_b = Buffer(1, 1024)

    process_tree(Sine(440), buffer_a)

    spectrum = np.fft.rfft(buffer_a.data[0])
    mags_py = np.abs(spectrum)[:512]
    angles_py = np.angle(spectrum)[:512]

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024)
    process_tree(fft, buffer_b)

    # Apple vDSP applies a scaling factor of 2x after forward FFT
    mags_out = np.copy(buffer_b.data[0][:512]) / 2
    angles_out = np.copy(buffer_b.data[0][512:])

    assert np.all(np.abs(angles_py[1:512] - angles_out[1:512]) < 0.0001)
    assert np.all(np.abs(mags_py[1:512] - mags_out[1:512]) < 0.0001)

def test_fft_ifft(graph):
    if no_fft:
        return

    #----------------------------------------------------------------
    # Verify that fft -> ifft returns the same output
    #----------------------------------------------------------------
    for buffer_size in [ 1024, 2048, 4096 ]:
        buffer_a = Buffer(1, buffer_size)
        buffer_b = Buffer(1, buffer_size)

        process_tree(Sine(440), buffer_a)

        fft = FFT(Sine(440), fft_size=1024, hop_size=1024)
        ifft = IFFT(fft)
        process_tree(ifft, buffer_b)

        assert np.all(np.abs(buffer_a.data[0] - buffer_b.data[0]) < 0.00001)

def test_fft_ifft_split(graph):
    if no_fft:
        return

    #----------------------------------------------------------------
    # Verify that fft -> ifft returns the same output, with output
    # buffer smaller than FFT size
    #----------------------------------------------------------------
    buffer_a = Buffer(1, 1024)
    buffer_b1 = Buffer(1, 512)
    buffer_b2 = Buffer(1, 512)
    buffer_b3 = Buffer(1, 512)

    process_tree(Sine(440), buffer_a)

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024)
    ifft = IFFT(fft)
    process_tree(ifft, buffer_b1)
    process_tree(ifft, buffer_b2)
    process_tree(ifft, buffer_b3)
    buffer_b_concatenate = np.concatenate((buffer_b2.data[0], buffer_b3.data[0]))

    assert np.all(buffer_b1.data[0] == 0)
    assert np.all(np.abs(buffer_a.data[0] - buffer_b_concatenate) < 0.00001)

def test_fft_convolve(graph):
    if no_fft:
        return

    buffer_a = Buffer(1, 1024)
    process_tree(Impulse(0), buffer_a)

    buffer_b = Buffer(1, 1024)
    process_tree(Sine(440), buffer_b)

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024)
    convolve = FFTConvolve(fft, buffer_a)
    ifft = IFFT(convolve) * 0.5
    buffer_c = Buffer(1, 1024)
    process_tree(ifft, buffer_c)

    # There is a consistent small difference between samples, perhaps due to a
    # timing delay after convolving?
    # print(buffer_b.data[0] - buffer_c.data[0])
    # [-0.01235887  0.01137974 -0.01235905 ...  0.01137954 -0.0123589 0.01137978]
    assert np.all(np.abs(buffer_b.data[0] - buffer_c.data[0]) < 0.015)

def test_fft_convolve_split(graph):
    if no_fft:
        return

    buffer_ir = Buffer(1, 4096)
    envelope_duration_seconds = buffer_ir.num_frames / graph.sample_rate
    envelope = EnvelopeASR(0, 0, envelope_duration_seconds) * Sine(440)
    process_tree(envelope, buffer_ir)

    fft = FFT(Impulse(0), fft_size=1024, hop_size=1024)
    convolve = FFTConvolve(fft, buffer_ir)
    ifft = IFFT(convolve) * 0.5

    buffer_out = Buffer(1, 1024)
    output_samples = np.zeros(0)
    for n in range(4):
        process_tree(ifft, buffer_out)
        output_samples = np.concatenate((output_samples, buffer_out.data[0]))
    assert np.all(np.abs(output_samples - buffer_ir.data[0]) < 0.015)
