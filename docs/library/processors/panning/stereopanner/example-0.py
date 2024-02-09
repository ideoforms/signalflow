from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using StereoPanner to pan a low pitch to the left and a high pitch to the
# right.
#-------------------------------------------------------------------------------
low = TriangleOscillator(220)
high = TriangleOscillator(660)

left = StereoPanner(low, -0.8)
right = StereoPanner(high, 0.8)

output = (left + right) * 0.5
output.play()
graph.wait()