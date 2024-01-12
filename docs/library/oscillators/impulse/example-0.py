from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Impulse generator producing an impulse every second (60bpm)
#-------------------------------------------------------------------------------
impulse = Impulse(frequency=1.0)
output = impulse * 0.5
output.play()