from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using an Impulse node as a clock to trigger an envelope once per second.
#-------------------------------------------------------------------------------
clock = Impulse(1.0)
osc = TriangleOscillator(250)
envelope = ASREnvelope(0.01, 0.0, 0.5, 1.0, clock)
output = StereoPanner(osc * envelope)
output.play()
graph.wait()