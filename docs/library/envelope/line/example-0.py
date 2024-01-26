from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using a line to control the gain of an oscillator, emulating a sidechain ducking effect.
#-------------------------------------------------------------------------------
clock = Impulse(frequency=1.0)
line = Line(0.0, 0.5, 1.0, 0, clock)
osc = SawOscillator(200)
output = osc * line
output.play()
graph.wait()