#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Modulation example.
#------------------------------------------------------------------------
from signalflow import *

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph()
graph.show_status(1)

#------------------------------------------------------------------------
# Create a sine oscillator, attenuate by 12dB, and pan to stereo.
#------------------------------------------------------------------------
clock = Impulse(8)
frequency = ScaleLinExp(Saw(0.2), -1, 1, 200, 2000)
sample_hold = SampleAndHold(frequency, clock)
sine = Triangle(sample_hold) * 0.5
env = EnvelopeASR(attack=0.001, sustain=0.001, release=0.1, clock=clock)
mono = sine * env
stereo = LinearPanner(2, mono)
delay1 = CombDelay(mono, 0.1, 0.8)
delay2 = OneTapDelay(CombDelay(mono, 0.05, 0.8), 0.125)
stereo = stereo + ChannelArray([ delay1, delay2 ]) * 0.2

#------------------------------------------------------------------------
# Play the output
#------------------------------------------------------------------------
graph.play(stereo)
graph.start()
graph.wait()
