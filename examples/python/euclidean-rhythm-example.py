#!/usr/bin/env python3

from signalflow import *
import random

graph = AudioGraph(start=True)

eu0 = Euclidean(Impulse(4), 24, 7)
flt0 = SVFFilter(eu0, "band_pass", cutoff=80, resonance=0.997) * 9

eu1 = Euclidean(Impulse(8), 13, 9)
flt1 = SVFFilter(eu1, "band_pass", cutoff=800, resonance=0.98) * 0.2

eu2 = Euclidean(Impulse(8), 16, 11)
flt2 = SVFFilter(eu2, "band_pass", cutoff=8000, resonance=0.97) * 0.1

eu3 = Euclidean(Impulse(4), 19, 12)
reson = RandomExponential(0.99, 0.999, eu3)
flt3 = SVFFilter(eu3, "band_pass", cutoff=480, resonance=reson) * 0.2

graph.play(LinearPanner(2, flt0, 0.0))
graph.play(LinearPanner(2, flt1, -0.5))
graph.play(LinearPanner(2, flt2, 0.5))
graph.play(LinearPanner(2, flt3, 0))

graph.wait()
