from signalflow import *
graph = AudioGraph()

# Looping envelope different curve shapes
sine = SineOscillator(880)
envelope = Envelope(levels=[0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0],
                    times =[0.2, 0.0, 0.2, 0.0, 0.2, 0.0, 0.2],
                    curves=[0.05, 1.0, 0.2, 1.0, 1.0, 1.0, 10.0], loop=True)
output = sine * envelope
output.play()

graph.wait()
