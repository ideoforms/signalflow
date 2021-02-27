#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Euclidean rhythm example, using a global pulse
# and ClockDivider to drive a series of rhythm generators.
# Impulses are passed through resonant filters of different freqencies.
#------------------------------------------------------------------------
from signalflow import *
import random

graph = AudioGraph(start=True)

clock = Impulse(10)
summer = Sum()

eu0 = Euclidean(ClockDivider(clock, 2), 24, 7)
flt0 = SVFFilter(eu0, "band_pass", cutoff=80, resonance=0.99) * 9
summer.add_input(LinearPanner(2, flt0, 0.0))

eu1 = Euclidean(clock, 13, 9)
flt1 = SVFFilter(eu1, "band_pass", cutoff=800, resonance=0.98) * 0.2
summer.add_input(LinearPanner(2, flt1, -0.5))

eu2 = Euclidean(clock, 16, 11)
flt2 = SVFFilter(eu2, "band_pass", cutoff=8000, resonance=0.97) * 0.1
summer.add_input(LinearPanner(2, flt2, 0.5))

eu3 = Euclidean(ClockDivider(clock, 2), 19, 12)
reson = RandomExponential(0.99, 0.999, eu3)
flt3 = SVFFilter(eu3, "band_pass", cutoff=480, resonance=reson) * 0.2
summer.add_input(LinearPanner(2, flt3, 0))

tanh = Tanh(summer * 20)
tanh.play()

graph.wait()
