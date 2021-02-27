#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Euclidean rhythm example, using a global pulse
# and ClockDivider to drive a series of rhythm generators.
# Impulses are passed through resonant filters of different freqencies.
#------------------------------------------------------------------------
from signalflow import *
import random

graph = AudioGraph(start=True)

class EuclideanPatch (Patch):
    def __init__(self, clock, divider=1, length=24, events=8, cutoff=8000, resonance=0.99, pan=0.0, amp=0.5):
        super().__init__()
        self.clock = clock
        self.divider = ClockDivider(self.clock, divider)
        self.eu = Euclidean(self.divider, length, events)
        self.flt = SVFFilter(self.eu, "low_pass", cutoff=cutoff, resonance=resonance)
        self.panned = LinearPanner(2, self.flt * amp, pan)
        self.set_output(self.panned)

clock = Impulse(8)
EuclideanPatch(clock, 2, 23, 7, 80, 0.99, 0.0, 20.0).play()
EuclideanPatch(clock, 1, 13, 9, 800, 0.98, 0.7, 0.2).play()
EuclideanPatch(clock, 1, 16, 11, 8000, 0.97, -0.7, 0.1).play()
EuclideanPatch(clock, 2, 19, 12, 480, 0.99, 0.0, 0.2).play()

graph.wait()
