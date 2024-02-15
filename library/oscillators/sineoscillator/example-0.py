from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Simple sine wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
sine = SineOscillator(440)
envelope = ASREnvelope(0.1, 0.1, 0.5)
output = StereoPanner(sine * envelope) * 0.5
output.play()
graph.wait()