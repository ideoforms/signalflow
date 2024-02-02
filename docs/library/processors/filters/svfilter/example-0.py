from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using SVFilter as a low-pass filter on white noise.
#-------------------------------------------------------------------------------
noise = WhiteNoise()
filtered = SVFilter(noise, SIGNALFLOW_FILTER_TYPE_LOW_PASS, 1000, 0.6)
output = StereoPanner(filtered)
output.play()
graph.wait()