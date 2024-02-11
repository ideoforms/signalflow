from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using Resample to distort a sine wave.
#-------------------------------------------------------------------------------
sine = SineOscillator(440)
crushed = Resample(sine, 11025, 4)
output = StereoPanner(crushed) * 0.3
output.play()
graph.wait()