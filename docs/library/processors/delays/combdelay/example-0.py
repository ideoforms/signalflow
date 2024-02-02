from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using CombDelay to change the character of a saw wave oscillator.
#-------------------------------------------------------------------------------
clock = Impulse(4)
arpeggio = [60, 62, 64, 66, 68, 70,
            72, 70, 68, 66, 64, 62]
sequence = Sequence(arpeggio, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = SawOscillator(frequency)
envelope= ASREnvelope(0.1, 0, 0.2, 1.0, clock)
voice = oscillator * envelope

comb = CombDelay(voice, 0.09, 0.6, 0.9)

output = StereoPanner(comb) * 0.5
output.play()
graph.wait()