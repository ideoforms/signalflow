from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Create a range of different waveshaper buffers, and iterate through them.
#-------------------------------------------------------------------------------
import time
import math
import random

sin_buf = WaveShaperBuffer(lambda n: math.sin(n * math.pi / 2))
cos_buf = WaveShaperBuffer(lambda n: math.cos(n * math.pi / 2))
tan_buf = WaveShaperBuffer(lambda n: math.tan(n * math.pi / 2))
tanh_buf = WaveShaperBuffer(lambda n: math.tanh(n * 20))
sinx_buf = WaveShaperBuffer(lambda n: math.sin(256 * n * math.pi / 2))
invert_buf = WaveShaperBuffer(lambda n: 1 - n if n > 0 else -1 - n)
noise_buf = WaveShaperBuffer(lambda n: random.uniform(0, n))
bufs = [sin_buf, cos_buf, tan_buf, tanh_buf, sinx_buf, invert_buf, noise_buf]

sine = SineOscillator(120)
waveshaper = WaveShaper(sine, sin_buf)
attenuated = waveshaper * 0.1
attenuated.play()

for buf in bufs:
    waveshaper.set_buffer("buffer", buf)
    time.sleep(1.0)

attenuated.stop()
