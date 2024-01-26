from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using an ASR Envelope to shape a square wave oscillator
#-------------------------------------------------------------------------------
osc = SquareOscillator(500)
envelope = ASREnvelope(0.1, 0.0, 0.5)
output = osc * envelope
output.play()
graph.wait()
