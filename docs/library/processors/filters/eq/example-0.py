from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using EQ to shape white noise. The low band (below 500Hz) is reduced. The mid
# band is boosted. The high band (above 2000Hz) is reduced drastically.
#-------------------------------------------------------------------------------
noise = WhiteNoise()

eq = EQ(noise, 0.6, 1.5, 0.1, 500, 2000)

output = StereoPanner(eq) * 0.5
output.play()
graph.wait()