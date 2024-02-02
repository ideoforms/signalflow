from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using SVFilter as a low-pass filter to reduce the harshness of a square wave
# oscillator.
#-------------------------------------------------------------------------------
clock = Impulse(3.5)

Am7 = [ 67, 64, 60, 57 ] * 4
D7 = [ 62, 66, 69, 72] * 4
arpeggios = Am7 + D7
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = SquareOscillator(frequency)
envelope= ASREnvelope(0.1, 0, 0.2, 1.0, clock)
voice = oscillator * envelope 
filtered = SVFilter(voice, SIGNALFLOW_FILTER_TYPE_LOW_PASS, 4000, 0.3)
delayed = AllpassDelay(filtered, 0.15, 0.8, 0.5)

pan = TriangleLFO(0.1, -1.0, 1.0)

output = StereoPanner(delayed, pan)
output.play()

graph.wait()