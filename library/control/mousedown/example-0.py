from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# When the left mouse button is clicked, as detected by MouseDown(), an LFO is applied to the oscillator's frequency.
#-------------------------------------------------------------------------------
lfo = SineLFO(5, 100, 600)
frequency = If(MouseDown(), lfo, 100)
osc = TriangleOscillator(frequency)
osc.play()
graph.wait()