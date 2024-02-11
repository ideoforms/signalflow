from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using SVFilter as a low-pass filter on white noise.
#-------------------------------------------------------------------------------
noise = WhiteNoise()
filtered = SVFilter(input=noise,
                    filter_type="low_pass", 
                    cutoff=1000, 
                    resonance=0.6)
output = StereoPanner(filtered)
output.play()
graph.wait()