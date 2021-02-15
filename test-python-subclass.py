#!/usr/bin/env python3

import numpy as np
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

graph = AudioGraph(start=True)
graph.show_status(2)

node = Sine()
graph.play([ node, node ])
while True:
    time.sleep(1.0)
