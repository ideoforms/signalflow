from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using white noise to control the pitch of an oscillator.
# A new pitch is determined once every second. Interpolation is turned off so 
# that the oscillator jumps to the new pitch instead of smoothly moving to it.
# Random interval is turned off so that pitch changes occur at a regular rate.
#-------------------------------------------------------------------------------
frequency = WhiteNoise(1, 100, 1000, False, False)
oscillator = SineOscillator(frequency)
output = StereoPanner(oscillator)
output.play()
graph.wait()