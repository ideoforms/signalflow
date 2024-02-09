from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using AllpassDelay to add a dreamy atmosphere to synth arpeggios
#-------------------------------------------------------------------------------
clock = Impulse(3.5)
Am7 = [ 67, 64, 60, 57 ] * 4
D7 = [ 62, 66, 69, 72] * 4
arpeggios = Am7 + D7
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = SquareOscillator(frequency)
envelope = ASREnvelope(0.1, 0, 0.2, 1.0, clock)
voice = oscillator * envelope
filtered = SVFilter(voice, "low_pass", 4000, 0.3)
delayed = AllpassDelay(input=filtered, 
                       delay_time=0.15, 
                       feedback=0.8)

pan = TriangleLFO(0.1, -1.0, 1.0)
output = StereoPanner(delayed, pan) * 0.5
output.play()
graph.wait()