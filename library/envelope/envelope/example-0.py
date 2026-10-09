from signalflow import *
graph = AudioGraph()

# Looping envelope different curve shapes
sine = SineOscillator(880)
envelope = Envelope(levels=[0.0, 1.0, 0.0], times=[0.5, 0.5], curves=[2, 0.5], loop=True)
output = sine * envelope
output.play()

graph.wait()
