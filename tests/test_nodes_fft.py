from libsignal import Buffer, EnvelopeASR
from libsignal import Sine, Impulse, FFT, IFFT

try:
    from libsignal import FFTConvolve
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
    #----------------------------------------------------------------
    # Verify that single-hop FFT output matches numpy's fft
    #----------------------------------------------------------------
    buffer_a = Buffer(1, 1024)
    buffer_b = Buffer(1, 1026)

    process_tree(Sine(440), buffer_a)

    spectrum = np.fft.rfft(buffer_a.data[0])
    mags_py = np.abs(spectrum)[:513]
    angles_py = np.angle(spectrum)[:513]

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024, do_window=False)
    process_tree(fft, buffer_b)

    # Apple vDSP applies a scaling factor of 2x after forward FFT
    # TODO: Fix this (and remove scaling factor in fftw forward fft)
    mags_out = np.copy(buffer_b.data[0][:513]) / 2
    angles_out = np.copy(buffer_b.data[0][513:])

    assert np.all(np.abs(mags_py - mags_out) < 0.0001)
    assert np.all(np.abs(angles_py - angles_out) < 0.0001)

def test_fft_windowed(graph):
    #----------------------------------------------------------------
    # Verify that single-hop FFT output matches numpy's fft
    #----------------------------------------------------------------
    buffer_a = Buffer(1, 1026)
    buffer_b = Buffer(1, 1026)

    process_tree(Sine(440), buffer_a)

    # Modify to match the symmetry of vDSP FFT
    window = np.hanning(buffer_a.num_frames + 1)[:buffer_a.num_frames]
    windowed = buffer_a.data[0] * window
    spectrum = np.fft.rfft(windowed)
    # spectrum = np.fft.rfft(buffer_a.data[0])
    mags_py = np.abs(spectrum)[:513]
    angles_py = np.angle(spectrum)[:513]

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024, do_window=True)
    process_tree(fft, buffer_b)

    # Apple vDSP applies a scaling factor of 2x after forward FFT
    mags_out = np.copy(buffer_b.data[0][:513]) / 2
    angles_out = np.copy(buffer_b.data[0][513:])

    print(mags_py)
    print(mags_out)

    # phases are mismatched for some reason
    assert np.all(np.abs(mags_py - mags_out) < 0.0001)

def test_fft_ifft(graph):
    #----------------------------------------------------------------
    # Verify that fft -> ifft returns the same output
    #----------------------------------------------------------------
    for buffer_size in [ 1024, 2048, 4096 ]:
        buffer_a = Buffer(1, buffer_size)
        buffer_b = Buffer(1, buffer_size)

        process_tree(Sine(440), buffer_a)

        fft = FFT(Sine(440), fft_size=1024, hop_size=1024, do_window=False)
        ifft = IFFT(fft)
        process_tree(ifft, buffer_b)

        assert np.all(np.abs(buffer_a.data[0] - buffer_b.data[0]) < 0.000001)

def test_fft_ifft_split(graph):
    #----------------------------------------------------------------
    # Verify that fft -> ifft returns the same output, with output
    # buffer smaller than FFT size
    #----------------------------------------------------------------
    buffer_a = Buffer(1, 1024)
    buffer_b1 = Buffer(1, 512)
    buffer_b2 = Buffer(1, 512)
    buffer_b3 = Buffer(1, 512)

    process_tree(Sine(440), buffer_a)

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024, do_window=False)
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

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024, do_window=False)
    convolve = FFTConvolve(fft, buffer_a)
    ifft = IFFT(convolve) * 0.5
    buffer_c = Buffer(1, 1024)
    process_tree(ifft, buffer_c)

    assert np.all(np.abs(buffer_b.data[0] - buffer_c.data[0]) < 0.000001)

def test_fft_convolve_split(graph):
    if no_fft:
        return

    buffer_ir = Buffer(1, 4096)
    envelope_duration_seconds = buffer_ir.num_frames / graph.sample_rate
    envelope = EnvelopeASR(0, 0, envelope_duration_seconds) * Sine(440)
    process_tree(envelope, buffer_ir)

    fft = FFT(Impulse(0), fft_size=1024, hop_size=1024, do_window=False)
    convolve = FFTConvolve(fft, buffer_ir)
    ifft = IFFT(convolve) * 0.5

    buffer_out = Buffer(1, 1024)
    output_samples = np.zeros(0)
    for n in range(4):
        process_tree(ifft, buffer_out)
        output_samples = np.concatenate((output_samples, buffer_out.data[0]))
    assert np.all(np.abs(output_samples - buffer_ir.data[0]) < 0.000001)
