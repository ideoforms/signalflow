from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using StereoPanner to repeatedly pan an arpeggiating oscillator between the 
# left and right channels.
#-------------------------------------------------------------------------------

clock = Impulse(8.0)
CMaj7 = [ 60, 64, 67, 71, 74, 76 ] * 8
FMaj9 = [ 65, 69, 72, 76, 77, 81 ] * 8
arpeggios = CMaj7 + FMaj9
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = TriangleOscillator(frequency)
release = Line(0.1, 0.5, 12, True)
envelope= ASREnvelope(0.0, 0.0, release, 1.0, clock)
voice = oscillator * envelope

pan = SineLFO(0.1667, -1.0, 1.0)

output = StereoPanner(voice, pan)
output.play()

graph.wait()