from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using the MouseY position to change the frequency of an oscillator.
#-------------------------------------------------------------------------------
frequency = MouseY() * 1000
osc = TriangleOscillator(frequency)
osc.play()
graph.wait()