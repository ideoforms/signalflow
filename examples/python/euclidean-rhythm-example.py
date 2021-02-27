#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Euclidean rhythm example, using a global pulse
# and ClockDivider to drive a series of rhythm generators.
# Impulses are passed through resonant filters of different freqencies.
#------------------------------------------------------------------------
from signalflow import *
import random

graph = AudioGraph(start=True)

patch = Patch()
divider = patch.add_input("divider", 1.0)
amp = patch.add_input("amp", 1.0)
clock = patch.add_input("clock", 0)
length = patch.add_input("length", 24)
events = patch.add_input("events", 8)
cutoff = patch.add_input("frequency", 8000)
resonance = patch.add_input("resonance", 0.97)
pan = patch.add_input("pan", 0.0)
clock = ClockDivider(clock, divider)
eu = patch.add_node(Euclidean(clock, length, events))
flt = SVFFilter(eu, "band_pass", cutoff=cutoff, resonance=resonance)
panned = LinearPanner(2, flt * amp, pan)
patch.set_output(panned)
spec = patch.create_spec()

clock = Impulse(8)
Patch(spec, { "clock" : clock, "divider": 2, "length": 24,  "events": 7, "frequency": 80, "resonance": 0.99, "pan": 0.0, "amp": 20 }).play()
Patch(spec, { "clock" : clock, "divider": 1, "length": 13, "events": 9, "frequency": 800, "resonance": 0.98, "pan": 0.7, "amp": 0.2 }).play()
Patch(spec, { "clock" : clock, "divider": 1, "length": 16, "events": 11, "frequency": 8000, "resonance": 0.97, "pan": -0.7, "amp": 0.1 }).play()
Patch(spec, { "clock" : clock, "divider": 2, "length": 19, "events": 12, "frequency": 480, "resonance": 0.99, "amp": 0.2 }).play()

graph.wait()
