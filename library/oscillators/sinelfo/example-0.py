from signalflow import *
graph = AudioGraph()

# Siren effect, using a sinewave LFO to modulate a sawtooth's frequency
lfo = SineLFO(1, 200, 1000)
saw = SawOscillator(lfo)
saw.play()
graph.wait()
