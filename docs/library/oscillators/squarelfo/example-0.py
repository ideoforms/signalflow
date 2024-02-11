from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Alarm effect, using a pulse wave LFO to modulate a sinewave's frequency
#-------------------------------------------------------------------------------
lfo = SquareLFO(1, 200, 400)
sine = SineOscillator(lfo)
output = StereoPanner(sine) * 0.5
output.play()
graph.wait()