from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Simple saw wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
saw = SawOscillator(440)
envelope = ASREnvelope(0.05, 0.1, 0.5)
output = StereoPanner(saw * envelope) * 0.5
output.play()
graph.wait()