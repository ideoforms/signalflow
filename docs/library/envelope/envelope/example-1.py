from signalflow import *
graph = AudioGraph()

# Percussive envelope with sharp attack and gradual decay, triggered by an impulse
noise = WhiteNoise()
impulse = Impulse(0.5)
envelope = Envelope(levels=[0.0, 1.0, 0.1, 0.0], times=[0.01, 0.1, 1.0], clock=impulse)
output = noise * envelope
output.play()

graph.wait()
