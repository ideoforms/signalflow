from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Creating a sequence using the MIDI note values of a C Major scale, starting on middle C.
#-------------------------------------------------------------------------------
clock = Impulse(2.0)
sequence = Sequence([ 60, 62, 64, 65, 67, 69, 71, 72 ], clock)
frequency = MidiNoteToFrequency(sequence)
oscillator = TriangleOscillator(frequency)
oscillator.play()
graph.wait()