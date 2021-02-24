#!/usr/bin/env python3

#--------------------------------------------------------------------------------
# Example: Audio through
#--------------------------------------------------------------------------------

from signalflow import *

graph = AudioGraph()

audio_in = AudioIn()

fft = FFT(audio_in * 0.1, 2048, 1024, 1024, do_window=False)
# sine = Sine(440) * EnvelopeASR(0, 0, 0.1) * 0.1
# fft = FFT(sine * 0.1, 1024, 512, 512, do_window=False)
ir_buffer = Buffer("/Users/daniel/Projects/Chirp/Impulse Responses/impulse_responses/spaces/RT60 3 - 5/3.8875_EchoBridge.wav")
convolve = FFTConvolve(fft, ir_buffer);
output = IFFT(convolve) * 0.1
# output = ChannelArray([ output, output ])
# sine = Sine([ 440, 441 ]) * 0.1
# sine = sine + output

graph.play(output)
graph.start()
try:
    graph.wait()
except KeyboardInterrupt:
    pass
