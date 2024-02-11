from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using StereoWidth to continuously alter the width of a stereo signal.
#-------------------------------------------------------------------------------
low = TriangleOscillator(220)
high = TriangleOscillator(660)
panned = ChannelArray([low, high])
width = StereoWidth(panned, TriangleLFO(0.5, 0, 1)) * 0.3
width.play()
graph.wait()