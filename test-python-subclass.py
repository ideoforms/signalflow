#!/usr/bin/env python3

import numpy as np
import random
import time
import math
from signalflow import *

class TestNode (Node):
    def __init__(self):
        # Only works when getattr() magic is disabled
        Node.__init__(self)
        self.phase = 0

    def process(self, frames):
        self.output_buffer[0][:frames] = np.sin((np.arange(frames) + self.phase) * 440 * np.pi * 2 / 44100.0)
        self.phase += frames

class NoiseNode (Node):
    def __init__(self):
        # Only works when getattr() magic is disabled
        Node.__init__(self)
        self.phase = 0
        self.amp = 1.0

    def process(self, frames):
        self.output_buffer[0][:frames] = np.random.uniform(-1, 1, frames)
        self.phase += 1
        if self.phase % 8 == 0:
            self.amp = random.uniform(0, 1)
        self.output_buffer[0][:frames] *= self.amp

class NoiseGraphNode:
    def __init__(self):
        self.noise = WhiteNoise()
        self.amp_mod = ScaleLinExp(RandomUniform(clock=Impulse(8)), 0, 1, 0.01, 1.0)
        self.output = self.noise * self.amp_mod * [0.1, 0.1]

    def play(self):
        self.output.play()

graph = AudioGraph(start=True)
graph.show_status(2)

node = Sine()
# graph.play([ node, node ])

noisenode = NoiseNode()
graph.play(noisenode * [0.1, 0.1])

# noisegraphnode = NoiseGraphNode()
# noisegraphnode.play()

graph.wait()
