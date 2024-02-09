from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using EQ to shape white noise. The low band (below 500Hz) is reduced. The mid
# band is boosted. The high band (above 2000Hz) is reduced drastically.
#-------------------------------------------------------------------------------
noise = WhiteNoise()
eq = EQ(input=noise, 
        low_gain=0.0, 
        mid_gain=1.5, 
        high_gain=0.2, 
        low_freq=1000, 
        high_freq=2000)
output = StereoPanner(eq) * 0.5
output.play()
graph.wait()