#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Modulation example.
#------------------------------------------------------------------------
from signalflow import *
import os

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph(start=True)

#------------------------------------------------------------------------
# Load an audio buffer.
#------------------------------------------------------------------------
audio_path = os.path.join(os.path.dirname(__file__), "../audio/gliss.aif")
buf = Buffer(audio_path)
clock = RandomImpulse(50)
pos = WhiteNoise(1, min=0, max=buf.duration, interpolate=False)
pan = RandomUniform(-1, 1, clock=clock)
rate = RandomExponential(0.1, 10, clock=clock)
granulator = Granulator(buf, clock, pan=pan, pos=pos, rate=rate, duration=0.1)

#------------------------------------------------------------------------
# Play the output.
#------------------------------------------------------------------------
graph.play(granulator)
graph.wait()
