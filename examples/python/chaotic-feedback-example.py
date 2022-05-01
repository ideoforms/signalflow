#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Chaotic feedback from FM self-modulation.
#------------------------------------------------------------------------
from signalflow import *

graph = AudioGraph()
graph.poll(1)

#------------------------------------------------------------------------
# Change frequency on average once per second.
#------------------------------------------------------------------------
f0 = RandomExponential(40, 2000, clock=RandomImpulse(1))

#------------------------------------------------------------------------
# Allocate a one-second buffer for use in the feedback line.
#------------------------------------------------------------------------
buf = Buffer(1, graph.sample_rate)

#------------------------------------------------------------------------
# op1 self-modulates by sending its output value to a
# FeedbackBufferWriter, and reading its previous outputs from a
# FeedbackBufferReader.
#------------------------------------------------------------------------
feedback = FeedbackBufferReader(buf)
op0 = SineOscillator(f0 + f0 * feedback * 14)

#------------------------------------------------------------------------
# Alter the level of self-oscillation once per second.
# Pass an array to RandomImpulse to generate different randomised 
# sequences in the L/R channels, for stereo output.
#------------------------------------------------------------------------
level = Smooth(RandomUniform(0, 1, clock=RandomImpulse([0.5, 0.5])))
level = ScaleLinExp(level, 0, 1, 0.0001, 1.0)
op0 = op0 * level
graph.add_node(FeedbackBufferWriter(buf, op0, 0.5))

#------------------------------------------------------------------------
# Use the chaotic output of op0 to modulate a second sine oscillator.
#------------------------------------------------------------------------
op1 = SineOscillator(f0 + f0 * op0 * 14)

graph.play(op1)
graph.wait()
