from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using the MouseX position to change the rate of an LFO, which is modulating an
# oscillator's frequency.
#-------------------------------------------------------------------------------
lfo_rate = MouseX() * 10
frequency = SineLFO(lfo_rate, 100, 600)
osc = TriangleOscillator(frequency)
osc.play()
graph.wait()    