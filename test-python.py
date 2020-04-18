#!/usr/bin/env python3 

import sys
import time
sys.path.append("build")

from libsignal import *

g = AudioGraph()
g.start()
# c = Constant(240.0)
s = Sine(440.0) + 0.1
# t = Sine(2) + 0.5
g.add_output(s)
time.sleep(2)
