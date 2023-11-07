#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Modulation example.
#------------------------------------------------------------------------
from signalflow import *
import os

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph()
graph.poll(2)

#------------------------------------------------------------------------
# Load an audio buffer.
#------------------------------------------------------------------------
audio_path = os.path.join(os.path.dirname(__file__), "audio", "gliss.aif")
buf = Buffer(audio_path)
clock = RandomImpulse(50)
pos = WhiteNoise(1, min=0, max=buf.duration, interpolate=False)
pan = RandomUniform(-1, 1, clock=clock)
rate = RandomExponential(0.5, 2.0, clock=clock)
granulator = Granulator(buf, clock, pan=pan, pos=pos, rate=rate, duration=0.5)
envelope = EnvelopeBuffer("triangle")
granulator.set_buffer("envelope", envelope)

#------------------------------------------------------------------------
# Play the output.
#------------------------------------------------------------------------
graph.play(granulator)
graph.wait()
