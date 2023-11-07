#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Modulation example.
#------------------------------------------------------------------------
from signalflow import *

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph()
graph.poll(1)

#------------------------------------------------------------------------
# Create a regular impulse that is used to trigger an envelope and S&H.
#------------------------------------------------------------------------
clock = Impulse(8)
frequency = ScaleLinExp(SawLFO(0.2), 0, 1, 200, 2000)
sample_hold = SampleAndHold(frequency, clock)
sine = TriangleOscillator(sample_hold) * 0.5
env = ASREnvelope(attack=0.001, sustain=0.001, release=0.1, clock=clock)

#------------------------------------------------------------------------
# Apply the envelope, and stereo pan between speakers.
#------------------------------------------------------------------------
mono = sine * env
stereo = StereoPanner(mono, SineLFO(0.5, -1, 1))

#------------------------------------------------------------------------
# Add some delay.
#------------------------------------------------------------------------
delay1 = CombDelay(mono, 0.1, 0.8)
delay2 = OneTapDelay(CombDelay(mono, 0.05, 0.8), 0.125)
stereo = stereo + ChannelArray([ delay1, delay2 ]) * 0.2

#------------------------------------------------------------------------
# Play the output.
#------------------------------------------------------------------------
graph.play(stereo)
graph.wait()
