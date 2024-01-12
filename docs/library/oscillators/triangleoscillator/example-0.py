from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Simple triangle wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
tri = TriangleOscillator(440)
envelope = ASREnvelope(0.1, 0.1, 0.5)
output = tri * envelope
output.play()
