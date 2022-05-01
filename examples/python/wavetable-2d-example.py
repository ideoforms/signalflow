#!/usr/bin/env python3 

#------------------------------------------------------------------------
# SignalFlow: Wavetable 2D example.
#------------------------------------------------------------------------
from signalflow import *
import numpy as np

graph = AudioGraph()
graph.poll(1)

wavetable_size = 4096

#------------------------------------------------------------------------
# Create a wavetable buffer for a band-limited square wave.
# A Buffer can be populated from a 1D numpy array.
#------------------------------------------------------------------------
square = np.zeros(wavetable_size)
for n in range(1, 300, 2):
    square += np.sin(np.arange(wavetable_size) * n * np.pi * 2.0 / wavetable_size) / n
square_buf = Buffer(1, wavetable_size, np.array([ square ]))

#------------------------------------------------------------------------
# Band-limited saw wave.
#------------------------------------------------------------------------
saw = np.zeros(wavetable_size)
for n in range(1, 300):
    saw += np.sin(np.arange(wavetable_size) * n * np.pi * 2.0 / wavetable_size) / n
saw_buf = Buffer(1, wavetable_size, np.array([ saw ]))

#------------------------------------------------------------------------
# Buffer2D is a wavetable representation that can interpolate between
# multiple different wavetables, using Wavetable2D for playback.
#------------------------------------------------------------------------
buffer2D = Buffer2D([square_buf, saw_buf])
frequency = SineLFO(0.1, 60, 61)
crossfade = SineLFO(0.5)
wavetable = Wavetable2D(buffer2D, frequency, crossfade) * 0.1

graph.play([wavetable, wavetable])
graph.wait()
