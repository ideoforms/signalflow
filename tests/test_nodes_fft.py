try:
    from libsignal import Sine, FFT, IFFT
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

    buffer_a = Buffer(1, 1024)
    buffer_b = Buffer(1, 1024)

    sine = Sine(440)
    process_tree(sine, buffer_a)

    spectrum = np.fft.rfft(buffer_a.data[0])
    mags_py = np.abs(spectrum)[:512] * 2
    angles_py = np.angle(spectrum)[:512]

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024)
    process_tree(fft, buffer_b)
    mags_out = np.copy(buffer_b.data[0][:512])
    angles_out = np.copy(buffer_b.data[0][512:])

    assert np.all(np.abs(angles_py[1:512] - angles_out[1:512]) < 0.0001)
    assert np.all(np.abs(mags_py[1:512] - mags_out[1:512]) < 0.0001)

def test_fft_ifft(graph):
    if no_fft:
        return

    buffer_a = Buffer(1, 1024)
    buffer_b = Buffer(1, 1024)

    sine = Sine(440)
    process_tree(sine, buffer_a)

    fft = FFT(Sine(440), fft_size=1024, hop_size=1024)
    ifft = IFFT(fft)
    process_tree(ifft, buffer_b)

    assert np.all(np.abs(buffer_a.data[0] - buffer_b.data[0]) < 0.000001)