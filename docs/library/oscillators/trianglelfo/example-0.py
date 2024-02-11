from signalflow import *
graph = AudioGraph()

#-----------------------------------------------------------------------------------
# Pacman ghost sound, using a triangle wave LFO to modulate a sine wave's frequency
#-----------------------------------------------------------------------------------
lfo = TriangleLFO(3, 200, 900)
sine = SineOscillator(lfo)
output = StereoPanner(sine) * 0.5
output.play()
graph.wait()