#!/usr/bin/env python3 

import sys
import time
sys.path.append("build")

from libsignal import *

g = AudioGraph()
g.start()

t = Sine(2) * 0.5 + 0.5
f = Sine(1.5) * 440 + 660
# s = Sine(440.0) * t
s = Sine(f) * t
g.add_output(s)
time.sleep(2)
