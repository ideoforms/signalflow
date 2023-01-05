from signalflow import Buffer, ASREnvelope
from signalflow import SineOscillator, Impulse, FFT, IFFT

try:
    from signalflow import FFTConvolve
    no_fft = False
except:
    #----------------------------------------------------------------
    # Skip these tests on platforms without FFT support
    #----------------------------------------------------------------
    no_fft = True

from . import graph
from . import process_tree

import numpy as np

fft_size = 2048
num_bins = fft_size // 2 + 1

def test_fft(graph):
    #--------------------------------------------------------------------------------
    # Verify that single-hop FFT output matches numpy's fft
    # 1026 = 512 bins plus DC and Nyquist
    #--------------------------------------------------------------------------------
    buffer_a = Buffer(1, fft_size)
    buffer_b = Buffer(1, num_bins * 2)

    sine = SineOscillator(440)
    graph.play(sine)
    graph.render_to_buffer(buffer_a)
    graph.stop(sine)

    spectrum = np.fft.rfft(buffer_a.data[0])
    mags_py = np.abs(spectrum)[:num_bins]
    angles_py = np.angle(spectrum)[:num_bins]

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
    process_tree(fft, buffer_b, len(buffer_b))

    # Apple vDSP applies a scaling factor of 2x after forward FFT
    # TODO: Fix this (and remove scaling factor in fftw forward fft)
    mags_out = np.copy(buffer_b.data[0][:num_bins]) / 2
    angles_out = np.copy(buffer_b.data[0][num_bins:])

    assert np.all(np.abs(mags_py - mags_out) < 0.001)
    assert np.all(np.abs(angles_py - angles_out) < 0.001)

def test_fft_windowed(graph):
    #--------------------------------------------------------------------------------
    # Verify that single-hop FFT output matches numpy's fft
    #--------------------------------------------------------------------------------
    buffer_a = Buffer(1, fft_size)
    buffer_b = Buffer(1, num_bins * 2)

    process_tree(SineOscillator(440), buffer_a)

    # Modify to match the symmetry of vDSP FFT
    window = np.hanning(buffer_a.num_frames + 1)[:buffer_a.num_frames]
    windowed = buffer_a.data[0] * window
    spectrum = np.fft.rfft(windowed)
    mags_py = np.abs(spectrum)[:num_bins]
    angles_py = np.angle(spectrum)[:num_bins]

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=True)
    process_tree(fft, buffer_b)

    # Apple vDSP applies a scaling factor of 2x after forward FFT
    mags_out = np.copy(buffer_b.data[0][:num_bins]) / 2
    angles_out = np.copy(buffer_b.data[0][num_bins:])

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

        process_tree(SineOscillator(440), buffer_a)

        fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
        ifft = IFFT(fft)
        process_tree(ifft, buffer_b)

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

    process_tree(SineOscillator(440), buffer_a)

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
    ifft = IFFT(fft)
    process_tree(ifft, buffer_b1)
    process_tree(ifft, buffer_b2)
    process_tree(ifft, buffer_b3)
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
    process_tree(Impulse(0), buffer_a)

    buffer_b = Buffer(1, fft_size)
    process_tree(SineOscillator(440), buffer_b)

    fft = FFT(SineOscillator(440), fft_size=fft_size, hop_size=fft_size, do_window=False)
    convolve = FFTConvolve(fft, buffer_a)
    ifft = IFFT(convolve) * 0.5
    buffer_c = Buffer(1, fft_size)
    process_tree(ifft, buffer_c)

    assert np.all(np.abs(buffer_b.data[0] - buffer_c.data[0]) < 0.000001)

def test_fft_convolve_split(graph):
    if no_fft:
        return

    buffer_ir = Buffer(1, fft_size * 4)
    envelope_duration_seconds = buffer_ir.num_frames / graph.sample_rate
    envelope = ASREnvelope(0, 0, envelope_duration_seconds) * SineOscillator(440)
    process_tree(envelope, buffer_ir)

    fft = FFT(Impulse(0), fft_size=fft_size, hop_size=fft_size, do_window=False)
    convolve = FFTConvolve(fft, buffer_ir)
    ifft = IFFT(convolve) * 0.5

    buffer_out = Buffer(1, fft_size)
    output_samples = np.zeros(0)
    for n in range(4):
        process_tree(ifft, buffer_out)
        output_samples = np.concatenate((output_samples, buffer_out.data[0]))
    assert np.all(np.abs(output_samples - buffer_ir.data[0]) < 0.000001)
