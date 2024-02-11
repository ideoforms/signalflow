from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Siren effect, using a sawtooth LFO to modulate a sinewave's frequency
#-------------------------------------------------------------------------------
lfo = SawLFO(1, 200, 1000)
sine = SineOscillator(lfo)
output = StereoPanner(sine) * 0.5
output.play()
graph.wait()
