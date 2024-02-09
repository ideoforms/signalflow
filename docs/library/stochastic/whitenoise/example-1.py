from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using white noise to simulate the sound of wind.
# White noise is generated at audio rate and passed into a band-pass filter.
# The cutoff of the filter is controlled by another white noise generator, which
# generates a new value between 100 and 300 at randomly-spaced intervals every 
# second, and smoothly interpolates between these values.
#-------------------------------------------------------------------------------
noise = WhiteNoise()
cutoff = WhiteNoise(1, 100, 300, True, True)
filtered = SVFilter(input=noise,
                    filter_type= "band_pass", 
                    cutoff=cutoff,
                    resonance=0.8)
output = StereoPanner(filtered) * 0.5
output.play()
graph.wait()