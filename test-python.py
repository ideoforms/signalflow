#!/usr/bin/env python3 

from libsignal import *

graph = AudioGraph()
graph.start()

amp = Sine(2) * 0.5 + 0.5
freq = Sine(1.5) * 440 + 660
output = Sine(freq) * amp
graph.add_output(output)
graph.wait()
