from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using OneTapDelay to bring controlled rhythmic interest to a melodic sequence
#-------------------------------------------------------------------------------
clock = Impulse(3.5)
Dm = [ 62, 65, 69 ] * 2
Bdim = [ 59, 62, 65 ] * 2
Gm = [55, 58, 62 ] * 2
Bb = [77, 74, 70 ]
A = [ 76, 73, 69 ]

arpeggios = Dm + Bdim + Gm + Bb + A
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = SquareOscillator(frequency)
envelope = ASREnvelope(0.1, 0, 0.2, 1.0, clock)
voice = oscillator * envelope
filtered = SVFilter(voice, "low_pass", 4000, 0.3)
delayed = filtered + OneTapDelay(filtered, 0.4) * 0.5

output = StereoPanner(delayed) * 0.3
output.play()
graph.wait()