from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using ChannelArray to pan a low tone to the left and a high tone to the right.
#-------------------------------------------------------------------------------
low = TriangleOscillator(220)
high = TriangleOscillator(660)
panned = ChannelArray([low, high]) * 0.3
panned.play()
graph.wait()